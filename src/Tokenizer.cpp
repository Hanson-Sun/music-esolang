#include <iostream>
#include "Tokenizer.h"
#include "../include/MidiReader.h"

bool Tokenizer::tokenize() {
    MidiReader midiReader;
    Midi midi = midiReader.read(file);

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

Token Tokenizer::chordToToken(std::vector<MidiNote> chord) {
    return Token();
}