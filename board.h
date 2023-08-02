#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <string>
#include <stack>

#define BOARD_ROWS 9
#define BOARD_COLUMNS 9
#define BOARD_BLOCKS 9
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
        inline void setRow(int16_t setRow);
        inline void setColumn(int16_t setColumn);
        inline void setValue(int16_t setValue);
        inline bool isUndefined();
        inline void print();
    private:
        int16_t row, column, value;
};

typedef std::stack<Position> Changelog;

class Square {
    public:
        Square();
        inline bool hasValue();
        inline int16_t getValue();
        bool setValue(int16_t setValue);
        int16_t nextValue(int16_t current);
        inline int16_t updateAvailable(int16_t unavailableValue, bool availability);
        inline bool isAvailable(int16_t unavailableValue);
        inline int16_t totalAvailable();
        inline bool* begin();
        inline bool* end();
    private:
        int16_t value;
        int16_t availableQuant;
        std::array<bool, MAX_VALUE + 1> available;
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
        std::string getBoardString();
        void print();
        inline bool insert(Position& pos, bool debug = 0);
        bool insert(int16_t row, int16_t column, int16_t value);
        inline void remove(Position& pos, bool debug = 0);
        void remove(int16_t row, int16_t column);
        inline Square* getSquare(Position& pos);
        void blockInfo();
        void solve(bool debug = 0);
    private:
        Square matrix[BOARD_ROWS][BOARD_COLUMNS];
        Changelog log;
        BlockInfo blocks;
        inline bool updateRow(int16_t row, int16_t value, bool availability);
        inline bool updateColumn(int16_t row, int16_t value, bool availability);
        inline bool updateBlock(int16_t block, int16_t value, bool availability);
        bool update(int16_t row, int16_t column, int16_t value, bool availability);
        bool inBounds(int16_t row, int16_t column);
        Position* minAvailability();
        bool recursiveSolver(bool debug = 0);
        void printLog();
};

#endif