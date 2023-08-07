#include "board.h"
#include <iostream>
#include <string.h>

#define NO_ARG 1
#define MAX_ARG 4

// Updates the flags given the current arguments.
void updateFlags(char** argv, bool* debugFlag, bool* printFlag) {
    if (!strcmp("-d", *argv)) {
        *debugFlag = true;
        return;
    } else if (!strcmp("-p", *argv)) {
        *printFlag = true;
        return;
    }
    throw std::runtime_error ("Argument did not match what was expected (must be \"-d\" or \"-p\")\n");
}

// Updates both flags to check for arguments.
// Returns the boardString to be used.
std::string parseInput(int argc, char** argv, bool* debugFlag, bool* printFlag) {
    *debugFlag = false;
    *printFlag = false;
    std::string bs;
    if (argc == NO_ARG)
        throw std::runtime_error ("Too few arguments.\n");
    if (argc > MAX_ARG)
        throw std::runtime_error ("Too many arguments.\n");
    argv++; // ignore program name
    switch (argc) {
        case 2: // only boardString
            bs = *argv;
            break;
        case 3: // 1 flag + boardString
            updateFlags(argv++, debugFlag, printFlag);
            bs = *argv;
            break;
        case 4: // 2 flags + boardString
            updateFlags(argv++, debugFlag, printFlag);
            updateFlags(argv++, debugFlag, printFlag);
            bs = *argv;
    }
    return bs;
}

int main(int argc, char** argv) {
    bool debugFlag, printFlag;
    std::string line = parseInput(argc, argv, &debugFlag, &printFlag);
    Board board;
    board.boardString(line);
    if (printFlag) board.print(); // print initial layout
    board.solve(debugFlag, printFlag); // solve the puzzle
    std::cout << board.getBoardString() << '\n'; // always display output boardString
    return 0;
}