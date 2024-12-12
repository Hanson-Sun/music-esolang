#include "Tokenizer.h"

#include <cmath>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "MidiReader.h"

const std::unordered_map<std::string, TokenType> CHORDTOKENMAP = {
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

Tokenizer::Tokenizer(Midi& m) : midi(m), groupIt(midi.begin()->group_begin()) {
    currentToken = chordToToken();
}

Tokenizer::TokenizerIterator& Tokenizer::TokenizerIterator::operator++() {
    tokenizer->currentToken = tokenizer->chordToToken();
    if (!(tokenizer->groupIt != tokenizer->midi.begin()->group_end())) {
        what++;
    }
    return *this;
}

Token Tokenizer::chordToToken() {
    TokenType type = peek();
    switch (type) {
        case TokenType::DIGIT:
            return chordToLiteral();
            break;
        case TokenType::COMMENT:
            return chordToComment();
            break;
        default:
            return chordToKeyword();
    }
}

std::string Tokenizer::formatCurrentChord() {
    auto chord = *groupIt;
    std::string chordLexeme = "(";
    int prev = -1;
    for (auto noteIt : chord) {
        const MidiNote& note = *noteIt;
        if (prev == -1) {
            prev = note.pitch;
        } else {
            chordLexeme += std::to_string(note.pitch - prev + 1) + ", ";
            prev = note.pitch;
        }
    }
    // std::cout << chordLexeme.substr(0, chordLexeme.size() - 2) + ")" << std::endl;
    return chordLexeme = chordLexeme.substr(0, chordLexeme.size() - 2) + ")";
}

TokenType Tokenizer::peek() {
    std::string chordLexeme = formatCurrentChord();
    if (chordLexeme == "()") {
        // std::cout << "oh" << std::endl;
        return TokenType::DIGIT;
    } else if (CHORDTOKENMAP.find(chordLexeme) != CHORDTOKENMAP.end()) {
        return CHORDTOKENMAP.at(chordLexeme);
    } else {
        return TokenType::DIGIT;
    }
}

Token Tokenizer::chordToComment() {
    std::vector<std::string> yeah;
    ++groupIt;
    while (groupIt != midi.begin()->group_end() && peek() != TokenType::COMMENT) {
        auto chord = *groupIt;
        for (auto noteIt : chord) {
            const MidiNote& note = *noteIt;
            yeah.push_back(pitchToNoteName(note.pitch));
            break;
        }
        ++groupIt;
    }
    ++groupIt;
    return Token(TokenType::COMMENT, std::to_string(base12toDecimal(yeah)));
}

Token Tokenizer::chordToLiteral() {
    std::vector<std::string> yeah;
    while (groupIt != midi.begin()->group_end() && peek() == TokenType::DIGIT) {
        auto chord = *groupIt;

        auto noteIt = chord[chord.size() - 1];
        yeah.push_back(pitchToNoteName((noteIt)->pitch));
        ++groupIt;
    }
    return Token(TokenType::LITERAL, std::to_string(base12toDecimal(yeah)));
}

int Tokenizer::base12toDecimal(std::vector<std::string> base12) {
    static const std::unordered_map<std::string, int> base12Map = {
        {"C", 0},  {"C#", 1}, {"D", 2},  {"D#", 3}, {"E", 4},   {"F", 5},
        {"F#", 6}, {"G", 7},  {"G#", 8}, {"A", 9},  {"A#", 10}, {"B", 11}};
    int decimal = 0;
    for (size_t i = 0; i < base12.size(); i++) {
        decimal += base12Map.at(base12[i]) * std::pow(12, base12.size() - 1 - i);
    }
    return decimal;
}

std::string Tokenizer::pitchToNoteName(int pitch) {
    static const std::string noteNames[12] = {"C",  "C#", "D",  "D#", "E",  "F",
                                              "F#", "G",  "G#", "A",  "A#", "B"};
    int index = pitch % 12;
    return noteNames[index];
}

Token Tokenizer::chordToKeyword() {
    std::string chordLexeme = formatCurrentChord();
    if (CHORDTOKENMAP.find(chordLexeme) != CHORDTOKENMAP.end()) {
        // std::cout << "what the " << chordLexeme << std::endl;
        ++(groupIt);
        return Token(CHORDTOKENMAP.at(chordLexeme), chordLexeme);
    } else {
        throw std::runtime_error("Invalid chord: " + chordLexeme);
    }
}
