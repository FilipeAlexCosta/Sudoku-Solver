# Sudoku-Solver
Making a sudoku solver to learn the basics of C++.

The current version is a simple brute force using recursion.
Still needs more tests.

Usage:
To execute it just call the program on the command line with a boardString (a string exactly 81 chars long
where a ' ' represents an empty space on the board, all other chars are numbers in the sudoku game).

Flags:
There are two possible flags you can use: "-d" and/or "-p".
Their relative order does not matter, but if used must appear before the boardString.

If "-p" is active then board is printed to the screen before and after being solved.

If "-d" is active, all insertions and removals will be printed during the solving process
in the format "Inserted: (row, column, value)" or "Removed: (row, column, value)".
In addition to that, all final moves will be printed from newest to oldest.