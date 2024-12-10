#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cmath>
#include "Tokenizer.h"
#include "../include/MidiReader.h"

std::unordered_map<std::string, TokenType> CHORDTOKENMAP = {
    {"(5)", TokenType::DEF},
    {"(5, 4)", TokenType::END},
    {"(13)", TokenType::COMMENT},
    {"(4)", TokenType::F},
    {"(8, 2)", TokenType::ADD},
    {"(8, 3)", TokenType::SUB},
    {"(8, 4)", TokenType::MUL},
    {"(8, 5)", TokenType::DIV},
    {"(8, 6)", TokenType::MOD},
    {"(7, 2)", TokenType::EQ},
    {"(7, 3)", TokenType::LESS},
    {"(7, 4)", TokenType::GREATER},
    {"(6, 2)", TokenType::AND},
    {"(6, 3)", TokenType::OR},
    {"(6, 4)", TokenType::NOT},
    {"(9, 2)", TokenType::POP},
    {"(9, 3)", TokenType::DUP},
    {"(9, 4)", TokenType::DUPDOT},
    {"(9, 5)", TokenType::SWAP},
    {"(9, 6)", TokenType::SIZE},
    {"(5, 4, 4)", TokenType::INPUT},
    {"(5, 4, 5)", TokenType::PRINT},
    {"(5, 4, 6)", TokenType::PRINTCHAR},
    {"(5, 4, 7)", TokenType::DEBUG},
    {"(5, 5, 4)", TokenType::IF},
    {"(5, 5, 5)", TokenType::ELSE},
    {"(5, 5, 6)", TokenType::WHILE},
    {"(8)", TokenType::VAR},
    {"(9)", TokenType::STORE},
    {"(10)", TokenType::LOAD},
    {"(11)", TokenType::FREE},
};

Tokenizer::Tokenizer(std::string file) : file(file) {
    midi = mr.read(file);
    auto groupBegin = midi.begin()->group_begin();
    groupIt = &groupBegin;
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
    // std::sort(chord.begin(), chord.end(), [](const MidiNote& a, const MidiNote& b) {
    //     return a.pitch < b.pitch;
    // });
    std::string chordLexeme = "(";
    for (auto noteIt : chord) {
        const MidiNote& note = *noteIt;
        chordLexeme += std::to_string(note.pitch) + ", ";
    }
    return chordLexeme = chordLexeme.substr(0, chordLexeme.size() - 2) + ")";
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
        // std::sort(chord.begin(), chord.end(), [](const MidiNote& a, const MidiNote& b) {
        //     return a.pitch > b.pitch;
        // });
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
        // std::sort(chord.begin(), chord.end(), [](const MidiNote& a, const MidiNote& b) {
        //     return a.pitch > b.pitch;
        // });
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
        decimal += base12Map[base12[i]] * std::pow(12, i);
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