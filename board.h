#ifndef BOARD_H
#define BOARD_H

#include <array>

#define BOARD_ROWS 9
#define BOARD_COLUMNS 9
#define UNFILLED 0
#define EMPTY_SQUARE ' '
#define MIN_VALUE 1
#define MAX_VALUE 9

class Position {
    public:
        Position(int8_t setRow, int8_t setColumn, int8_t setValue);
        inline int8_t getRow();
        inline int8_t getColumn();
        inline int8_t getValue();
    private:
        int8_t row, column, value;
};

class Square {
    public:
        Square();
        inline int8_t getValue();
        bool setValue(int8_t setValue);
        inline void updateAvailable(int8_t unavailableValue, bool availability);
        inline bool isAvailable(int8_t unavailabeValue);
        inline int8_t totalAvailable();
    private:
        int8_t value;
        int8_t availableQuant;
        std::array<bool, MAX_VALUE + MIN_VALUE> available;
};

class Board {
    public:
        Board();
        void print();
        void insert(Position& pos);
        void insert(int8_t row, int8_t column, int8_t value);
    private:
        Square matrix[BOARD_ROWS][BOARD_COLUMNS];
        void updateRow(int8_t row, int8_t value, bool availability);
        void updateColumn(int8_t row, int8_t value, bool availability);
        void updateBlock();
};

#endif