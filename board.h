#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <string>

#define BOARD_ROWS 9
#define BOARD_COLUMNS 9
#define BOARD_BLOCKS 9
#define UNFILLED 0
#define EMPTY_SQUARE ' '
#define MIN_VALUE 1
#define MAX_VALUE 9
#define UNDEFINED -1

class Position {
    public:
        Position();
        Position(int16_t setRow, int16_t setColumn, int16_t setValue);
        inline int16_t getRow();
        inline int16_t getColumn();
        inline int16_t getValue();
        inline void print();
    private:
        int16_t row, column, value;
};

class Square {
    public:
        Square();
        inline int16_t getValue();
        bool setValue(int16_t setValue);
        inline void updateAvailable(int16_t unavailableValue, bool availability);
        inline bool isAvailable(int16_t unavailabeValue);
        inline int16_t totalAvailable();
    private:
        int16_t value;
        int16_t availableQuant;
        std::array<bool, MAX_VALUE + MIN_VALUE> available;
};

class Block {
    public:
        Block();
        inline void append(Position pos);
        inline Position* begin();
        inline Position* end();
    private:
        Position positions[MAX_VALUE + MIN_VALUE - 1];
        int16_t size;
};

class BlockInfo {
    public:
        BlockInfo();
        inline int16_t getBlock(Position& pos);
        inline int16_t getBlock(int16_t row, int16_t column);
        inline Position* begin(int16_t block);
        inline Position* end(int16_t block);
        void printBlockBoard();
        void printBlocks();
    private:
        Block blocks[BOARD_BLOCKS];
        int16_t blockBoard[BOARD_BLOCKS][BOARD_COLUMNS];
};

class Board {
    public:
        Board();
        void boardString(std::string& str);
        void print();
        inline void insert(Position& pos);
        void insert(int16_t row, int16_t column, int16_t value);
        void blockInfo();
    private:
        Square matrix[BOARD_ROWS][BOARD_COLUMNS];
        BlockInfo blocks;
        inline void updateRow(int16_t row, int16_t value, bool availability);
        inline void updateColumn(int16_t row, int16_t value, bool availability);
        inline void updateBlock(int16_t block, int16_t value, bool availability);
        void update(int16_t row, int16_t column, int16_t value, bool availability);
        bool inBounds(int16_t row, int16_t column);
};

#endif