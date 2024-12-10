#include <iostream>
#include "Tokenizer.h"
#include "../include/MidiReader.h"

// bool Tokenizer::tokenize() {
//     MidiReader midiReader;
//     Midi midi = midiReader.read(file);

//     for (auto& track : midi) {
//         for (auto groupIt = track.group_begin(); groupIt != track.group_end(); ++groupIt) {
//             // TODO
//             auto chord = *groupIt;
//             std::sort(chord.begin(), chord.end(), [](const MidiNote& a, const MidiNote& b) {
//                 return a.pitch < b.pitch;
//             });
//             std::string cname = "(";
//             int lastPitch = -1;
//             for (auto noteIt : chord) {
//                 if (lastPitch != -1) {
//                     const MidiNote& note = *noteIt;
//                     cname += ", " + std::to_string(note.pitch - lastPitch);
//                 }
//             }
//         }
//     }
//     return 0;
// }

Tokenizer::Tokenizer(std::string file) : file(file) {
    midi = mr.read(file);
    groupIt = &midi.begin()->group_begin();
    currentToken = chordToKeyword();
    if (currentToken.type == TokenType::DIGIT) {
        // TODO parse literal
        std::cerr << "Invalid chord: " << currentToken.chordLexeme << std::endl;
    }
}

std::string Tokenizer::formatCurrentChord() {
    auto chord = **groupIt;
    std::sort(chord.begin(), chord.end(), [](const MidiNote& a, const MidiNote& b) {
        return a.pitch < b.pitch;
    });
    std::string chordLexeme = "(";
    for (auto noteIt : chord) {
        const MidiNote& note = *noteIt;
        chordLexeme += std::to_string(note.pitch) + ", ";
    }
    chordLexeme = chordLexeme.substr(0, chordLexeme.size() - 2) + ")";
}

TokenType Tokenizer::peek() {
    std::string chordLexeme = formatCurrentChord();
    if (CHORDTOKENMAP.find(chordLexeme) != CHORDTOKENMAP.end()) {
        return CHORDTOKENMAP[chordLexeme];
    } else {
        return TokenType::DIGIT;
    }
}

Token Tokenizer::chordToLiteral() {
    auto it = *groupIt;
    while(it != midi.begin()->group_end() && peek() == TokenType::DIGIT) {
        std::string chordLexeme = formatCurrentChord();
        ++groupIt;
    }
}

Token Tokenizer::chordToKeyword() {
    std::string chordLexeme = formatCurrentChord();
    if (CHORDTOKENMAP.find(chordLexeme) != CHORDTOKENMAP.end()) {
        return Token(CHORDTOKENMAP[chordLexeme], chordLexeme);
    } else {
        throw std::runtime_error("Invalid chord: " + chordLexeme);
    }
}