# Sudoku-Solver
Making a sudoku solver to learn the basics of C++.

The current version is a simple brute force using recursion.
Still needs more tests.

Usage:
To execute it just call the program on the command line with a boardString (a string exactly 81 chars long
where a ' ' represents an empty space on the board, all other chars are numbers in the sudoku game).

Example:
./compiled_program_name "4     7   9 1       3  96 8  2    1 6  5  8 2     8 7   6  19 3       2 5   4    "
Should yield: 465823791798165234123479658872634519631597842954218376246751983317986425589342167

There are two possible flags you can use: "-d" and/or "-p".
Their relative order does not matter, but if used must appear before the boardString.

If "-p" is active then board is printed to the screen before and after being solved.
Example:
./compiled_program_name -p "4     7   9 1       3  96 8  2    1 6  5  8 2     8 7   6  19 3       2 5   4    "
Should yield:
"

4    |     |7    
  9  |1    |     
    3|    9|6   8
-----------------
    2|     |  1  
6    |5    |8   2
     |    8|  7  
-----------------
    6|    1|9   3
     |     |  2  
5    |  4  |     


4 6 5|8 2 3|7 9 1
7 9 8|1 6 5|2 3 4
1 2 3|4 7 9|6 5 8
-----------------
8 7 2|6 3 4|5 1 9
6 3 1|5 9 7|8 4 2
9 5 4|2 1 8|3 7 6
-----------------
2 4 6|7 5 1|9 8 3
3 1 7|9 8 6|4 2 5
5 8 9|3 4 2|1 6 7

465823791798165234123479658872634519631597842954218376246751983317986425589342167
"

If "-d" is active, all insertions and removals will be printed during the solving process
in a format Inserted: (row, column, value) or Removed: (row, column, value)
In addition to that, all final moves will be printed from newest to oldest.

Example:
./compiled_program_name -d "4     7   9 1       3  96 8  2    1 6  5  8 2     8 7   6  19 3       2 5   4    "
Should yield:
"
(...)
Inserted: (4, 2, 1)
Inserted: (4, 1, 3)
Inserted: (4, 4, 9)
Inserted: (3, 4, 3)
Inserted: (0, 4, 8)
Removed: (0, 4, 8)
Removed: (3, 4, 3)
Removed: (4, 4, 9)
Removed: (4, 1, 3)
(...)
7: (2, 8, 8)
6: (2, 6, 6)
5: (2, 5, 9)
4: (2, 2, 3)
3: (1, 3, 1)
2: (1, 1, 9)
1: (0, 6, 7)
0: (0, 0, 4)

465823791798165234123479658872634519631597842954218376246751983317986425589342167
"