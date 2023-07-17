#include "board.h"
#include <iostream>

// *****************************************************************************
// Position methods
// *****************************************************************************

// Initializes a positon.
Position::Position(int8_t setRow, int8_t setColumn, int8_t setValue) {
    row = setRow;
    column = setColumn;
    value = setValue;
}

// Returns the row of a position.
inline int8_t Position::getRow() {
    return row;
}

// Returns the column of a position.
inline int8_t Position::getColumn() {
    return column;
}

// Returns the value of a position.
inline int8_t Position::getValue() {
    return value;
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
inline int8_t Square::getValue() {
    return value;
}

// Sets the value of the square.
// Returns true if a new value was set, false otherwise.
// If the value is not in range, throws std::runtime_error.
bool Square::setValue(int8_t setValue) {
    if (setValue < MIN_VALUE || setValue > MAX_VALUE)
        throw std::runtime_error("Square::setValue: value out of range.");
    if (setValue != UNFILLED && !isAvailable(setValue))
        return false;
    value = setValue;
    return true;
}

// Removes a possible value for the square.
// If the value is out of range, causes undefined behaviour.
inline void Square::updateAvailable(int8_t unavailableValue, bool availability) {
    available[unavailableValue] = availability;
    availableQuant = availableQuant - 1 * (!availability);
}

// Returns whether a value is available or not.
// If the value is out of range, causes undefined behaviour.
inline bool Square::isAvailable(int8_t unavailabeValue) {
    return available[unavailabeValue];
}

// Returns the total number of available values for the square.
inline int8_t Square::totalAvailable() {
    return availableQuant;
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

// Prints the sudoku board.
void Board::print() {
    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < (BOARD_COLUMNS - 1); j++) {
            if (matrix[i][j].getValue() != UNFILLED)
                std::cout << matrix[i][j].getValue() << '-';
            else
                std::cout << EMPTY_SQUARE << '-';
        }
        if (matrix[i][BOARD_COLUMNS - 1].getValue() != UNFILLED)
            std::cout << matrix[i][BOARD_COLUMNS - 1].getValue() << '\n';
        else
            std::cout << EMPTY_SQUARE << '\n';
    }
}

// Inserts a new value in the Board.
void Board::insert(Position& pos) {
    matrix[pos.getRow()][pos.getColumn()].setValue(pos.getValue());
}

// Inserts a new value in the Board.
void Board::insert(int8_t row, int8_t column, int8_t value) {
    matrix[row][column].setValue(value);
}

// Updates the availability of other squares in the same row.
void Board::updateRow(int8_t row, int8_t value, bool availability) {
    if (value < MIN_VALUE || value > MAX_VALUE)
        throw std::runtime_error("Board::updateRow: value out of range.");
    for (int j = 0; j < BOARD_COLUMNS; j++)
        matrix[row][j].updateAvailable(value, availability);
}

// Updates the availability of other squares in the same column.
void Board::updateColumn(int8_t column, int8_t value, bool availability) {
    if (value < MIN_VALUE || value > MAX_VALUE)
        throw std::runtime_error("Board::updateColumn: value out of range.");
    for (int i = 0; i < BOARD_ROWS; i++)
        matrix[i][column].updateAvailable(value, availability);
}