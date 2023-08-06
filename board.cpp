#include "board.h"
#include <iostream>

// *****************************************************************************
// Position methods
// *****************************************************************************

// Initializes an undefined position.
Position::Position() {
    row = UNDEFINED;
    column = UNDEFINED;
    value = UNDEFINED;
}

// Initializes a positon.
Position::Position(int16_t setRow, int16_t setColumn, int16_t setValue) {
    row = setRow;
    column = setColumn;
    value = setValue;
}

// Returns the row of a position.
inline int16_t Position::getRow() {
    return row;
}

// Returns the column of a position.
inline int16_t Position::getColumn() {
    return column;
}

// Returns the value of a position.
inline int16_t Position::getValue() {
    return value;
}

// Sets the row of a position.
inline void Position::setRow(int16_t setRow) {
    row = setRow;
}

// Sets the column of a position.
inline void Position::setColumn(int16_t setColumn) {
    column = setColumn;
}

// Sets the value of a position.
inline void Position::setValue(int16_t setValue) {
    value = setValue;
}

// Returns true if all fields of position are UNDEFINED.
inline bool Position::isUndefined() {
    return !(row != UNDEFINED || column != UNDEFINED || value != UNDEFINED);
}

// Prints a position.
inline void Position::print() {
    std::cout << '(' << row << ", " << column << ", " << value << ")\n";
}

// *****************************************************************************
// Square methods
// *****************************************************************************

// Initializes an empty square.
Square::Square() {
    value = UNDEFINED;
    availableQuant = MAX_VALUE - MIN_VALUE + 1;
    for (int i = 0; i < MIN_VALUE; i++)
        available[i] = false;
    for (int i = MIN_VALUE; i <= MAX_VALUE; i++)
        available[i] = true;
}

// Returns true if the square's value has been set, false otherwise.
inline bool Square::hasValue() {
    if (value < MIN_VALUE || value > MAX_VALUE) return false;
    return true;
}

// Returns the value of the square.
inline int16_t Square::getValue() {
    return value;
}

// Sets the value of the square.
// Returns true if a new value was set, false otherwise.
// If the value is not in range, throws std::runtime_error.
bool Square::setValue(int16_t setValue) {
    if ((setValue < MIN_VALUE || setValue > MAX_VALUE) && setValue != UNDEFINED)
        throw std::runtime_error("Square::setValue: value out of range.");
    if (setValue != UNDEFINED && !isAvailable(setValue))
        return false;
    value = setValue;
    return true;
}

// Returns the next value available.
// If there is no next value, UNDEFINED is returned.
int16_t Square::nextValue(int16_t current) {
    if (current == MAX_VALUE) return UNDEFINED;
    for (int16_t i = 1; current + i <= MAX_VALUE; i++)
        if (available[current + i]) return current + i;
    return UNDEFINED;
}

// Removes a possible value for the square.
// Returns the number of possibilities left.
// If the value is out of range, causes undefined behaviour.
inline int16_t Square::updateAvailable(int16_t unavailableValue, bool availability) {
    if (availability ^ available[unavailableValue]) // checks if a change actually happened
        availableQuant += -1 * (!availability) + 1 * availability;
    available[unavailableValue] = availability;
    return availableQuant;
}

// Returns whether a value is available or not.
// If the value is out of range, causes undefined behaviour.
inline bool Square::isAvailable(int16_t unavailableValue) {
    return available[unavailableValue];
}

// Returns the total number of available values for the square.
inline int16_t Square::totalAvailable() {
    return availableQuant;
}

// Returns an iterator to the beginning of the availability container.
inline bool* Square::begin() {
    return &available[MIN_VALUE];
}

// Returns the past-the-end iterator of the availability container.
inline bool* Square::end() {
    return (&available[MAX_VALUE]) + 1;
}

// *****************************************************************************
// BlockInfo methods
// *****************************************************************************

// Initializes a block. DOES NOT INITIALIZE THE POSITION ARRAY.
Block::Block() {
    size = 0;
}

// Appends a new position to the block.
inline void Block::append(Position pos) {
    if (size > MAX_VALUE + MIN_VALUE - 1)
        throw std::runtime_error("Block::append: to many positions in block.\n");
    positions[size] = pos;
    size++;
}

// Returns an iterator to the beginning of the block.
inline Position* Block::begin() {
    return &positions[0];
}

// Returns the past-the-end iterator of the block.
inline Position* Block::end() {
    Position* res = &positions[MAX_VALUE + MIN_VALUE - 2];
    res++;
    return res;
}

// *****************************************************************************
// BlockInfo methods
// *****************************************************************************

// Initializes blocks which represent all the positions belonging to
// a certain block and the blockBoard which maps row, column -> block.
BlockInfo::BlockInfo() {
    int blockWidth = BOARD_ROWS / 3;
    int blockHeight = BOARD_COLUMNS / 3;
    if (BOARD_ROWS != BOARD_COLUMNS)
        throw std::runtime_error("Board must be square.\n");
    if (!(BOARD_ROWS % 2))
        throw std::runtime_error("Board must have odd measurements.\n");
    for (int rowRepeat = 0, block = 0; rowRepeat < 3; rowRepeat++, block += 3) {
        for (int i = rowRepeat * blockHeight; i < (rowRepeat + 1) * blockHeight; i++) {
            int currentBlock = block;
            for (int columnRepeat = 0; columnRepeat < 3; columnRepeat++) {
                for (int j = columnRepeat * blockWidth; j < (columnRepeat + 1) * blockWidth; j++) {
                    Position newPos(i, j, UNDEFINED);
                    blockBoard[i][j] = currentBlock;
                    blocks[currentBlock].append(newPos);
                }
                currentBlock++;
            }
        }
    }
}

// Returns the block of a given position.
inline int16_t BlockInfo::getBlock(Position& pos) {
    return getBlock(pos.getRow(), pos.getColumn());
}

// Returns the block of a given position.
inline int16_t BlockInfo::getBlock(int16_t row, int16_t column) {
    return blockBoard[row][column];
}

// Returns an iterator to the first element of the block in blocks.
inline Position* BlockInfo::begin(int16_t block) {
    return blocks[block].begin();
}

// Returns the past-the-end iterator of the block in blocks.
inline Position* BlockInfo::end(int16_t block) {
    return blocks[block].end();
}

// Prints all the block groups to the cout.
void BlockInfo::printBlocks() {
    for (int i = 0; i < BOARD_BLOCKS; i++) {
        for (auto it = blocks[i].begin(); it != blocks[i].end(); ++it)
            it->print();
        std::cout << '\n';
    }
}

// Prints the blockBoard to the cout.
void BlockInfo::printBlockBoard() {
    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLUMNS; j++) {
            std::cout << blockBoard[i][j] << ' ';
        }
        std::cout << '\n';
    }
}

// *****************************************************************************
// Board methods
// *****************************************************************************

// Initializes an empty sudoku Board.
Board::Board() {
    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLUMNS; j++) {
            Square newSquare;
            matrix[i][j] = newSquare;
        }
    }
}

// Converts a board string into a board object.
void Board::boardString(std::string& str) {
    int16_t row = 0, column = 0;
    constexpr int boardSize = BOARD_ROWS * BOARD_COLUMNS;
    if (str.size() != boardSize)
        throw std::runtime_error("Board::boardString: too few/many elements to insert.\n");
    for (auto it = str.begin(); it != str.end(); ++it) {
        if ((*it) != EMPTY_SQUARE)
            insert(row, column, (*it) - '0');
        if (column != BOARD_COLUMNS - 1) {
            column++;
            continue;
        }
        column = 0;
        row++;
    }
}

std::string Board::getBoardString() {
    std::string res;
    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLUMNS; j++) {
            if (matrix[i][j].hasValue())
                res += matrix[i][j].getValue() + '0';
            else
                res += EMPTY_SQUARE;
        }
    }
    return res;
}

// Prints the sudoku board.
void Board::print() {
    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < (BOARD_COLUMNS); j++) {
            if (matrix[i][j].getValue() == UNDEFINED)
                std::cout << EMPTY_SQUARE;
            else
                std::cout << matrix[i][j].getValue();
            if ((j + 1) % 3) std::cout << ' ';
            if (!((j + 1) % 3) && j != BOARD_COLUMNS - 1) std::cout << '|';
        }
        std::cout << '\n';
        if (!((i + 1) % 3) && i != BOARD_ROWS - 1) {
            for (int k = 0; k < BOARD_COLUMNS + 8; k++)
                std::cout << '-';
            std::cout << '\n';
        }
    }
}

// Inserts a new value in the Board.
// Returns true if a contradiction was found, false otherwise.
inline bool Board::insert(Position& pos, bool debug) {
    if (debug) {
        std::cout << "Inserted: ";
        pos.print();
    }
    return insert(pos.getRow(), pos.getColumn(), pos.getValue());
}

// Inserts a new value in the Board.
// Returns true if a contradiction was found, false otherwise.
bool Board::insert(int16_t row, int16_t column, int16_t value) {
    matrix[row][column].setValue(value);
    Position newLog(row, column, value); // creates a new log to document
    log.push(newLog); // the change in the board
    return update(row, column, value, false);
}

// Removes a value in the Board.
inline void Board::remove(Position& pos, bool debug) {
    if (debug) {
        std::cout << "Removed: ";
        pos.print();
    }
    remove(pos.getRow(), pos.getColumn());
}

// Removes a value in the Board.
void Board::remove(int16_t row, int16_t column) {
    if (log.empty())
        throw std::runtime_error("Board::remove: cannot remove from an empty board.\n");
    if (log.top().getRow() != row && log.top().getColumn() != column)
        throw std::runtime_error("Board::remove: can only remove the last changed square.\n");
    update(row, column, matrix[row][column].getValue(), true);
    matrix[row][column].setValue(UNDEFINED);
    log.pop();
}

// Returns a reference to the square denoted by pos.
inline Square* Board::getSquare(Position& pos) {
    if (!inBounds(pos.getRow(), pos.getColumn()))
        throw std::runtime_error("Board::getSquare: position is out of board bounds.\n");
    return &matrix[pos.getRow()][pos.getColumn()];
}

// Updates the availability of all squares in the row.
// Returns true if a contradiction was encountered, false otherwise.
inline bool Board::updateRow(int16_t row, int16_t value, bool availability) {
    int16_t res = 0;
    for (int j = 0; j < BOARD_COLUMNS; j++) {
        // update the square and get the number of available values left
        int16_t squareRes = matrix[row][j].updateAvailable(value, availability);
        squareRes |= matrix[row][j].getValue(); // get the value of the square
        squareRes = !squareRes; // if the value is not set and there are no more possibilities
        res |= squareRes; // that means we have reached a contradiction
    }
    return static_cast<bool>(res);
}

// Updates the availability of all squares in the column.
// Returns true if a contradiction was encountered, false otherwise.
inline bool Board::updateColumn(int16_t column, int16_t value, bool availability) {
    int16_t res = 0;
    for (int i = 0; i < BOARD_ROWS; i++) {
        // update the square and get the number of available values left
        int16_t squareRes = matrix[i][column].updateAvailable(value, availability);
        squareRes |= matrix[i][column].getValue(); // get the value of the square
        squareRes = !squareRes; // if the value is not set and there are no more possibilities
        res |= squareRes; // that means we have reached a contradiction
    }
    return static_cast<bool>(res);
}

// Updates the availability of all squares in the block.
// Returns true if a contradiction was encountered, false otherwise.
inline bool Board::updateBlock(int16_t block, int16_t value, bool availability) {
    int16_t res = 0;
    for (auto it = blocks.begin(block); it != blocks.end(block); ++it) {
        // update the square and get the number of available values left
        int16_t squareRes = matrix[it->getRow()][it->getColumn()].updateAvailable(value, availability);
        squareRes |= matrix[it->getRow()][it->getColumn()].getValue(); // get the value of the square
        squareRes = !squareRes; // if the value is not set and there are no more possibilities
        res |= squareRes; // that means we have reached a contradiction
    }
    return (static_cast<bool>(res));
}

// Updates the availability of all squares in the affected area of the given row
// and column (including the associated block).
// Returns true if a contradiction is found (as in, empty square with no possible values).
bool Board::update(int16_t row, int16_t column, int16_t value, bool availability) {
    if (value < MIN_VALUE || value > MAX_VALUE)
        throw std::runtime_error("Board::update: value out of range.");
    if (!inBounds(row, column))
        throw std::runtime_error("Board::update: position out of board.\n");
    bool foundContradiction = updateRow(row, value, availability);
    foundContradiction |= updateColumn(column, value, availability);
    foundContradiction |= updateBlock(blocks.getBlock(row, column), value, availability);
    return foundContradiction;
}

// Checks whether or not a position is within bounds.
bool Board::inBounds(int16_t row, int16_t column) {
    if (row < 0 || row > BOARD_ROWS) return false;
    if (column < 0 || column > BOARD_COLUMNS) return false;
    return true;
}

// Returns the block info of the board.
void Board::blockInfo() {
    blocks.printBlocks();
    std::cout << "\n";
    blocks.printBlockBoard();
    std::cout << '\n';
}

// Solves the sudoku puzzle and prints it.
// If debug is true, prints the changelog in reverse.
void Board::solve(bool debug) {
    recursiveSolver(debug);
    if (debug) {
        printLog();
        std::cout << '\n';
    }
    print();
}

// Returns a reference to a position where the square with the least possibilities is.
// Returns an undefined position if all squares are filled.
// Remember to delete the position afterwards.
Position* Board::minAvailability() {
    Position* pos = new Position();
    int16_t min = MAX_VALUE - MIN_VALUE + 5; // random starting value above the total availability
    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLUMNS; j++) {
            if (matrix[i][j].hasValue()) continue; // rule out filled squares
            if (matrix[i][j].totalAvailable() >= min) continue; // only want the min
            pos->setRow(i);
            pos->setColumn(j);
            min = matrix[i][j].totalAvailable();
        }
    }
    return pos;
}

// Recursively solves the sudoku puzzle.
bool Board::recursiveSolver(bool debug) {
    Position* minPosition = minAvailability();
    if (minPosition->isUndefined()) return true; // If there are no squares left, it's solved
    Square* minSquare = getSquare(*minPosition);
    int16_t i = MIN_VALUE;
    for (auto it = minSquare->begin(); it != minSquare->end(); ++it, ++i) {
        if (!(*it)) continue; // if value is not available, skip it
        minPosition->setValue(i); // otherwise insert it. If the insertion does not cause a contradiction
        if (!insert(*minPosition, debug) && recursiveSolver(debug)) return true; // go down a layer
        remove(*minPosition, debug); // otherwise, if a contradiction appears, remove the inserted value
    }
    return false; // handles "nested" contradictions
}

// Destructively prints the changelog in reverse.
void Board::printLog() {
    while (!log.empty()) {
        std::cout << log.size() - 1 << ": "; // "idx: (row, col, val)"
        log.top().print();
        log.pop();
    }
}