#include "MidiReader.h"

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>

// Copy constructor
MidiTrack::GroupIterator::GroupIterator(const GroupIterator& other)
    : track(other.track),
      current(other.current),
      lastChord{other.lastChord},
      chordCache(other.chordCache) {}

// Copy assignment operator
MidiTrack::GroupIterator& MidiTrack::GroupIterator::operator=(const GroupIterator& other) {
    if (this != &other) {
        track = other.track;
        current = other.current;
        lastChord = other.lastChord;
        chordCache = other.chordCache;
    }
    return *this;
}

std::vector<MidiTrack::const_iterator> MidiTrack::findChord(const_iterator noteIt) const {
    std::vector<const_iterator> chord;
    const MidiNote& note = *noteIt;

    for (auto it = begin(); it != end(); ++it) {
        if (!(it->end_time <= note.start_time || it->start_time >= note.end_time)) {
            chord.push_back(it);
        }
    }

    // sorted here guh
    std::sort(chord.begin(), chord.end(),
              [](const_iterator a, const_iterator b) { return (*a).pitch > (*b).pitch; });

    return chord;
}

std::vector<MidiTrack::iterator> MidiTrack::findChord(iterator noteIt) {
    std::vector<iterator> chord;
    MidiNote& note = *noteIt;

    for (auto it = begin(); it != end(); ++it) {
        if (!(it->end_time <= note.start_time || it->start_time >= note.end_time)) {
            chord.push_back(it);
        }
    }

    // sorted here guh
    std::sort(chord.begin(), chord.end(),
              [](const_iterator a, const_iterator b) { return (*a).pitch < (*b).pitch; });

    return chord;
}

MidiTrack::GroupIterator::GroupIterator(TrackType& track, iterator start)
    : track(track), current(start) {
    if (current == track.begin()) {
        chordCache.clear();
    }

    lastChord = findChordWithCache(current);
}

void MidiTrack::GroupIterator::reset() {
    current = track.begin();
    chordCache.clear();
    if (current != track.end()) {
        lastChord = findChordWithCache(current);
    }
}

MidiTrack::GroupIterator& MidiTrack::GroupIterator::operator++() {
    do {
        if (chordCache.find(std::distance(track.begin(), current)) == chordCache.end()) {
            lastChord = findChordWithCache(current);
            break;
        }
        ++current;

        if (current == track.end()) {
            chordCache.clear();
        }
    } while (current != track.end());
    return *this;
}

std::vector<MidiTrack::iterator> MidiTrack::GroupIterator::findChordWithCache(
    MidiTrack::iterator noteIt) {
    size_t index = std::distance(track.begin(), noteIt);
    if (chordCache.find(index) != chordCache.end()) {
        chordCache.insert(index);
        return lastChord;
    }

    std::vector<iterator> chord = track.findChord(noteIt);

    for (auto it : chord) {
        chordCache.insert(std::distance(track.begin(), it));
    }

    return chord;
}

Midi MidiReader::read(const std::string& filename) {
    Midi midi;
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return midi;
    }

    char header[14];
    file.read(header, 14);
    if (std::string(header, 4) != "MThd") {
        std::cerr << "Invalid MIDI file: " << filename << std::endl;
        return midi;
    }

    int format = (header[8] << 8) | header[9];
    int ntrks = (header[10] << 8) | header[11];
    int division = (header[12] << 8) | header[13];

    midi.setFormat(format);
    midi.setDivision(division);

    if (format == 0 && ntrks != 1) {
        std::cerr << "Invalid number of tracks for Format 0: " << ntrks << std::endl;
        return midi;
    }

    for (int i = 0; i < ntrks; ++i) {
        char trackHeader[8];
        file.read(trackHeader, 8);
        if (std::string(trackHeader, 4) != "MTrk") {
            std::cerr << "Invalid track chunk in file: " << filename << std::endl;
            return midi;
        }

        int trackLength = (static_cast<uint8_t>(trackHeader[4]) << 24) |
                          (static_cast<uint8_t>(trackHeader[5]) << 16) |
                          (static_cast<uint8_t>(trackHeader[6]) << 8) |
                          (static_cast<uint8_t>(trackHeader[7]) << 0);

        if (trackLength <= 0) {
            std::cerr << "Invalid track length in file: " << filename << std::endl;
            return midi;
        }

        std::vector<char> trackData(trackLength);
        file.read(trackData.data(), trackLength);
        if (file.gcount() != trackLength) {
            std::cerr << "Error reading track data from file: " << filename << std::endl;
            return midi;
        }

        midi.addTrack(parseTrack(trackData));
    }

    return midi;
}

MidiTrack MidiReader::parseTrack(const std::vector<char>& trackData) {
    std::vector<MidiNote> track;
    size_t pos = 0;
    int current_time = 0;
    std::vector<MidiNote> active_notes(128, {-1, -1, -1, -1});  // To keep track of active notes

    while (pos < trackData.size()) {
        // read delta time
        int delta_time = 0;
        uint8_t byte;
        do {
            byte = trackData[pos++];
            delta_time = (delta_time << 7) | (byte & 0x7F);
        } while (byte & 0x80);
        current_time += delta_time;

        MidiNote note = parseMidiEvent(trackData, pos);
        if (note.start_time != -1) {  // on
            note.start_time = current_time;
            active_notes[note.pitch] = note;
        } else if (note.end_time != -1) {  // off
            note.end_time = current_time;
            if (active_notes[note.pitch].start_time != -1) {
                active_notes[note.pitch].end_time = current_time;
                track.push_back(active_notes[note.pitch]);
                active_notes[note.pitch] = {-1, -1, -1, -1};
            }
        }
    }
    return MidiTrack(track);
}

MidiNote MidiReader::parseMidiEvent(const std::vector<char>& trackData, size_t& pos) {
    MidiNote note = {0, 0, 0, 0};
    uint8_t status = trackData[pos++];

    if (status == 0xFF) {  // meta event??
        // uint8_t type = trackData[pos++];
        pos++;
        uint8_t length = trackData[pos++];
        pos += length;  // skip meta event data ??
        // if (type == 0x51 && length == 3) {  // this is really weird bruh
        //     uint32_t microseconds_per_quarter_note =
        //         (static_cast<uint8_t>(trackData[pos]) << 16) |
        //         (static_cast<uint8_t>(trackData[pos + 1]) << 8) |
        //         static_cast<uint8_t>(trackData[pos + 2]);
        //     tempo = 60000000 / microseconds_per_quarter_note;
        // }
    } else if (status >= 0x80 && status <= 0xEF) {  // MIDI event
        uint8_t param1 = trackData[pos++];
        uint8_t param2 = (status & 0xF0) != 0xC0 && (status & 0xF0) != 0xD0 ? trackData[pos++] : 0;

        if ((status & 0xF0) == 0x90 && param2 > 0) {  // Note on
            note = {0, -1, param1, param2};           // start time will be set later
        } else if ((status & 0xF0) == 0x80 ||
                   ((status & 0xF0) == 0x90 && param2 == 0)) {  // note off
            note = {-1, 0, param1, 0};                          // end time will be set later
        }
    }

    return note;
}
