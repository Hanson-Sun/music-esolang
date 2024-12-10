#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "Tokenizer.h"
#include "../include/MidiReader.h"

Tokenizer::Tokenizer(std::string file) : file(file) {
    midi = mr.read(file);
    groupIt = &midi.begin()->group_begin();
    currentToken = chordToToken();
}

Tokenizer Tokenizer::operator++() {
    currentToken = chordToToken();
    return *this;
}

Token Tokenizer::chordToToken() {
    TokenType type = peek();
    switch(type) {
        case TokenType::IDENTIFIER:
            return chordToIdentifier();
        case TokenType::NUMBER:
            return chordToNumber();
        default:
            return chordToKeyword();
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

Token Tokenizer::chordToIdentifier() {
    auto it = *groupIt;
    std::vector<std::string> yeah;
    while(it != midi.begin()->group_end() && peek() == TokenType::DIGIT) {
        auto chord = **groupIt;
        std::sort(chord.begin(), chord.end(), [](const MidiNote& a, const MidiNote& b) {
            return a.pitch > b.pitch;
        });
        // shutup
        for (auto noteIt : chord) {
            const MidiNote& note = *noteIt;
            yeah.push_back(pitchToNoteName(note.pitch));
            break;
        }
        ++it;
    }
    return Token(TokenType::IDENTIFIER, std::to_string(base12toDecimal(yeah)));
}

Token Tokenizer::chordToNumber() {
    auto it = *groupIt;
    std::vector<std::string> yeah;
    while(it != midi.begin()->group_end() && peek() == TokenType::DIGIT) {
        auto chord = **groupIt;
        std::sort(chord.begin(), chord.end(), [](const MidiNote& a, const MidiNote& b) {
            return a.pitch > b.pitch;
        });
        // shutup
        for (auto noteIt : chord) {
            const MidiNote& note = *noteIt;
            yeah.push_back(pitchToNoteName(note.pitch));
            break;
        }
        ++it;
    }
    return Token(TokenType::NUMBER, std::to_string(base12toDecimal(yeah)));
}

int Tokenizer::base12toDecimal(std::vector<std::string> base12) {
    std::unordered_map<std::string, int> base12Map = {{"C", 0}, {"C#", 1}, {"D", 2}, {"D#", 3}, {"E", 4}, {"F", 5}, {"F#", 6}, {"G", 7}, {"G#", 8}, {"A", 9}, {"A#", 10}, {"B", 11}};
    int decimal = 0;
    for (int i = 0; i<base12.size(); i++) {
        decimal += base12Map[base12[i]] * pow(12, i);
    }
    return decimal;
}

std::string Tokenizer::pitchToNoteName(int pitch) {
   std::string noteNames[12] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    int index = pitch % 12;
    return noteNames[index];
}

Token Tokenizer::chordToKeyword() {
    std::string chordLexeme = formatCurrentChord();
    if (CHORDTOKENMAP.find(chordLexeme) != CHORDTOKENMAP.end()) {
        return Token(CHORDTOKENMAP[chordLexeme], chordLexeme);
    } else {
        throw std::runtime_error("Invalid chord: " + chordLexeme);
    }
    ++(*groupIt);
}