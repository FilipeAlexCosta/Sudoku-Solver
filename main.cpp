#include "board.h"
#include <iostream>

int main() {
    Board myBoard;
    std::string line;
    std::getline(std::cin, line);
    myBoard.boardString(line);
    myBoard.print();
    return 0;
}