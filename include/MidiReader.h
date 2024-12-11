#ifndef MIDIREADER_H
#define MIDIREADER_H

#include <algorithm>
#include <string>
#include <unordered_set>
#include <vector>

struct MidiNote {
    int start_time;
    int end_time;
    int pitch;
    int velocity;
};

class MidiTrack {
 private:
    std::vector<MidiNote> notes;
    std::vector<std::vector<MidiNote>> chords;

 public:
    MidiTrack() {}
    explicit MidiTrack(const std::vector<MidiNote>& notes) : notes(notes) {};
    void addNote(const MidiNote& note) {notes.push_back(note);}
    const std::vector<MidiNote>& getNotes() const {return notes;}
    void setNotes(const std::vector<MidiNote>& new_notes) {notes = new_notes;}

    using iterator = std::vector<MidiNote>::iterator;
    using const_iterator = std::vector<MidiNote>::const_iterator;

    iterator begin() { return notes.begin(); }
    const_iterator begin() const { return notes.begin(); }
    iterator end() { return notes.end(); }
    const_iterator end() const { return notes.end(); }

    MidiNote& operator[](size_t index) { return notes[index]; }
    const MidiNote& operator[](size_t index) const { return notes[index]; }

    std::vector<const_iterator> findChord(const_iterator noteIt) const;
    std::vector<iterator> findChord(iterator noteIt);

    size_t size() const { return notes.size(); }

    class GroupIterator {
     private:
        using TrackType =
            typename std::conditional<std::is_const<iterator>::value, const MidiTrack, MidiTrack>::type;
        TrackType& track;
        iterator current;
        std::vector<iterator> lastChord;
        std::unordered_set<size_t> chordCache;

     public:
        using value_type = std::vector<iterator>;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;
        using iterator_category = std::forward_iterator_tag;

        GroupIterator(TrackType& track, iterator start);

        // Copy constructor
        GroupIterator(const GroupIterator& other);

        // Copy assignment operator
        GroupIterator& operator=(const GroupIterator& other);

        value_type operator*() const { return lastChord; }

        void reset();

        GroupIterator& operator++();

        bool operator!=(const GroupIterator& other) const {
            return current != other.current;
        }
     private:
        std::vector<iterator> findChordWithCache(iterator noteIt);
    };


    GroupIterator group_begin() { return GroupIterator(*this, begin()); }
    GroupIterator group_end() { return GroupIterator(*this, end()); }

    void resetGroupIterator(GroupIterator& groupIt) { groupIt.reset(); }
};

class Midi {
    using iterator = std::vector<MidiTrack>::iterator;
    using const_iterator = std::vector<MidiTrack>::const_iterator;

 private:
    int division;
    int format;
    std::vector<MidiTrack> tracks;

 public:
    Midi() : division(-30), format(1) {}
    void addTrack(const MidiTrack& track) { tracks.push_back(track); }
    void setDivision(int new_division) { division = new_division; }
    int getDivision() const { return division; }
    void setFormat(int new_format) { format = new_format; }
    int getFormat() const { return format; }
    const std::vector<MidiTrack>& getTracks() const { return tracks; }

    size_t size() const { return tracks.size(); }

    iterator begin() { return tracks.begin(); }
    const_iterator begin() const { return tracks.begin(); }
    iterator end() { return tracks.end(); }
    const_iterator end() const { return tracks.end(); }

    MidiTrack& operator[](size_t index) { return tracks[index]; }
    const MidiTrack& operator[](size_t index) const { return tracks[index]; }
};

class MidiReader {
 public:
    static Midi read(const std::string& filename);

 private:
    static MidiTrack parseTrack(const std::vector<char>& trackData);
    static MidiNote parseMidiEvent(const std::vector<char>& trackData, size_t& pos);
};
#endif
