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

// Prints a position.
inline void Position::print() {
    std::cout << '(' << row << ", " << column << ", " << value << ")\n";
}

// *****************************************************************************
// Square methods
// *****************************************************************************

// Initializes an empty square.
Square::Square() {
    value = UNFILLED;
    availableQuant = MAX_VALUE - MIN_VALUE + 1;
    for (int i = 0; i < MIN_VALUE; i++)
        available[i] = false;
    for (int i = MIN_VALUE; i < MAX_VALUE + MIN_VALUE; i++)
        available[i] = true;
}

// Returns the value of the square.
inline int16_t Square::getValue() {
    return value;
}

// Sets the value of the square.
// Returns true if a new value was set, false otherwise.
// If the value is not in range, throws std::runtime_error.
bool Square::setValue(int16_t setValue) {
    if (setValue < MIN_VALUE || setValue > MAX_VALUE)
        throw std::runtime_error("Square::setValue: value out of range.");
    if (setValue != UNFILLED && !isAvailable(setValue))
        return false;
    value = setValue;
    return true;
}

// Removes a possible value for the square.
// If the value is out of range, causes undefined behaviour.
inline void Square::updateAvailable(int16_t unavailableValue, bool availability) {
    available[unavailableValue] = availability;
    availableQuant = availableQuant - 1 * (!availability);
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
                    Position newPos(i, j, UNFILLED);
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

// Converts 
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

// Prints the sudoku board.
void Board::print() {
    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < (BOARD_COLUMNS); j++) {
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
inline void Board::insert(Position& pos) {
    insert(pos.getRow(), pos.getColumn(), pos.getValue());
}

// Inserts a new value in the Board.
void Board::insert(int16_t row, int16_t column, int16_t value) {
    matrix[row][column].setValue(value);
    update(row, column, value, false);
}

// Removes a value in the Board.
inline void Board::remove(Position& pos) {
    remove(pos.getRow(), pos.getColumn());
}

// Removes a value in the Board.
void Board::remove(int16_t row, int16_t column) {
    if (matrix[row][column].getValue() == UNDEFINED)
        throw std::runtime_error("Board::remove: the selected square was not set in the first place.\n");
    update(row, column, matrix[row][column].getValue(), true);
    matrix[row][column].setValue(UNDEFINED);
}

// Updates the availability of all squares in the row.
inline void Board::updateRow(int16_t row, int16_t value, bool availability) {
    for (int j = 0; j < BOARD_COLUMNS; j++)
        matrix[row][j].updateAvailable(value, availability);
}

// Updates the availability of all squares in the column.
inline void Board::updateColumn(int16_t column, int16_t value, bool availability) {
    for (int i = 0; i < BOARD_ROWS; i++)
        matrix[i][column].updateAvailable(value, availability);
}

// Updates the availability of all squares in the block.
inline void Board::updateBlock(int16_t block, int16_t value, bool availability) {
    for (auto it = blocks.begin(block); it != blocks.end(block); ++it)
        matrix[it->getRow()][it->getColumn()].updateAvailable(value, availability);
}

// Updates the availability of all squares in the affected area of the given row
// and column (including the associated block).
void Board::update(int16_t row, int16_t column, int16_t value, bool availability) {
    if (value < MIN_VALUE || value > MAX_VALUE)
        throw std::runtime_error("Board::update: value out of range.");
    if (!inBounds(row, column))
        throw std::runtime_error("Board::update: position out of board.\n");
    updateRow(row, value, availability);
    updateColumn(column, value, availability);
    updateBlock(blocks.getBlock(row, column), value, availability);
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