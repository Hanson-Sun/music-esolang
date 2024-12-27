#include "Tokenizer.h"
#include "Parser.h"
#include "Interpreter.h"
#include "MidiReader.h"
#include "Errors.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


void printHelp(const std::string& programName) {
    std::cout << "Usage: " << programName << " [options] [filename]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -h, --help                   Print this help message" << std::endl;
    std::cout << "  -p, --parse    [filename]    Print the parsed AST" << std::endl;
    std::cout << "  -t, --tokenize [filename]    Print the tokenized results" << std::endl;
    std::cout << "\nIf no MIDI file is provided, the interpreter will start in interactive REPL mode.\n";
}

void repl() {
    // gonna need to use rtMidi
    std::cout << "Starting interactive REPL mode. Type 'exit' or ctrl-c to quit." << std::endl;

    // this doesnt actually work
    Interpreter interpreter;
    while (true) {
        std::string value;
        std::cout << "NOT IMPLEMENTED" << std::endl;
        std::cin >> value;

        /**
        somehow get a list of the notes (live midi notes), users will control when a group of notes is ready

        MidiTrack track = MidiTrack(notes)
        Tokenizer tokenizer = Tokenizer(track);
        Parser(tokenizer);
        Parser::ParserIterator it = parser.begin();

        // somehow add smart detection for things like defs, control flow, and blocks, which will ask the user for more notes.
        while (it) {
            auto statement = *it;
            if (_check(statement)) {
                ErrorHandler::printError(std::get<Error>(statement));
                break;
            }
            
            if (auto result = interpreter.interpret(std::get<Statement_t>(statement)); _check(result)) {
                ErrorHandler::printError(std::get<Error>(result));
                break;
            }

            ++it;
        }
         */
    }
}

void run(const std::string midiFilename) {
    Midi midi;

    try {
        midi = MidiReader::read(midiFilename);
    } catch (const std::runtime_error& error) {
        std::cerr << "Error reading MIDI file '" << midiFilename << ": " << error.what() << std::endl;  
    }

    Tokenizer tokenizer(*midi.begin());
    Parser parser(tokenizer);
    Parser::ParserIterator it = parser.begin();

    Interpreter interpreter;

    while (it) {
        auto statement = *it;
        if (_check(statement)) {
            ErrorHandler::printError(std::get<Error>(statement));
            break;
        }
        
        if (auto result = interpreter.interpret(std::get<Statement_t>(statement)); _check(result)) {
            ErrorHandler::printError(std::get<Error>(result));
            break;
        }

        ++it;
    }
}

void printParsedResults(const std::string& filename) {
    Midi midi = MidiReader::read(filename);
    Tokenizer tokenizer(*midi.begin());

    ASTPrinter printer;

    Parser parser(tokenizer);
    Parser::ParserIterator it = parser.begin();
    while(it) {
        auto result = *it;
        if (_check(result)) {
            ErrorHandler::printError(std::get<Error>(result));
            break;
        }

        std::get<Statement_t>(result)->accept(printer);
        ++it;
    }
    std::cout << std::endl;
}

void printTokenizedResults(const std::string& filename) {
    Midi midi = MidiReader::read(filename);
    Tokenizer tokenizer(*midi.begin());
    Tokenizer::TokenizerIterator tokenizerIt = tokenizer.begin();
    while (tokenizerIt) {
        std::cout << (*tokenizerIt).toString() << " ";
        ++tokenizerIt;
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            printHelp(argv[0]);
            return 0;
        }
    }

    if (argc == 1) {
        repl();  
    } else if (std::string arg = std::string(argv[1]); (arg == "-p" || arg == "--parse") && argc == 3) {
        std::string midiFilename = argv[2];
        printParsedResults(midiFilename);
    } else if (std::string arg = std::string(argv[1]); (arg == "-t" || arg == "--tokenize") && argc == 3) {
        std::string midiFilename = argv[2];
        printTokenizedResults(midiFilename);
    } else if (argc == 2) {
        run(argv[2]);
    } else {
        std::cerr << "Incorrect parameters. " << std::endl;
        printHelp(argv[0]);
        return 1;
    }

    return 0;
}