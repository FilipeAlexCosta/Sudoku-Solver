#include "board.h"
#include <iostream>

int main() {
    Board myBoard;
    std::string line;
    std::getline(std::cin, line);
    myBoard.boardString(line);
    myBoard.solve(true);
    std::cout << "\nI: " << line << "\nO: " << myBoard.getBoardString() << '\n';
    return 0;
}