#include <iostream>
#include "../include/MidiReader.h"
#include "../include/Tokenizer.h"

int main() {
    Midi midi = MidiReader::read("../midi/chordTest.mid");

    for (auto& track : midi) {
        for (auto groupIt = track.group_begin(); groupIt != track.group_end(); ++groupIt) {
            auto chord = *groupIt;
            std::cout << "Chord or Note Group:" << std::endl;
            for (auto noteIt : chord) {
                const MidiNote& note = *noteIt;
                std::cout << "  Start: " << note.start_time << " End: " << note.end_time << " Pitch: " << note.pitch
                          << " Velocity: " << note.velocity << std::endl;
            }
        }
    }

    return 0;
}