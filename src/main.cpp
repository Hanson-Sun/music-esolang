#include "Tokenizer.h"
#include "Parser.h"
#include "Interpreter.h"
#include "MidiReader.h"
#include "Errors.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

void printHelp(const char* programName) {
    std::cout << "Usage: " << programName << " [options] [midi_file.mid]\n";
    std::cout << "Options:\n";
    std::cout << "  -h, --help\t\tDisplay this help message\n";
    std::cout << "  [midi_file.mid]\tPath to a MIDI file to interpret\n";
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

int main(int argc, char* argv[]) {

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            printHelp(argv[0]);
            return 0;
        }
    }

    if (argc >= 2) {
        std::string midiFilename = argv[1];
        run(midiFilename);
    } else if (argc == 1) {
        repl();
    } else {
        std::cerr << "Incorrect parameters. " << std::endl;
        printHelp(argv[0]);
        return 1;
    }
    return 0;
}