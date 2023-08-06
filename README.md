# Sudoku-Solver
Making a sudoku solver to learn the basics of C++.

The current version is a simple brute force using recursion.
Still needs more tests.

***bugs to be fixed***
I think there is a bug with recursion that is being masked by the algorithm's
ability to find the square with least possible available numbers.

The obvious bug is that the availability of a value is changed no matter what,
even in backtracking.
Basically, imagine the algorithm places a 6 somewhere, it then communicates
to all squares affected that they cannot be 6's anymore. If the recursion continues
and finds a contradiction and backtracks all the way to that 6 and removes it,
it will then communicate again to those 6 that they can be 6's again, even
if they are affected by other 6's which obviously allows for duplicates.
To fix this I have two approaches:
    1. Instead of keeping track of things, each time I choose a square I check to see
    which values are available.
    2. Instead of keeping track of available values in a boolean fashion, each square
    can keep track of how many of each number it is affected by. Let's say there is
    an unplayed square that in the recursive process has 2 5's affecting it, then in
    the available array it would display in the 5th place a -2, which means that if
    those 1 of those 5's happened to be removed, it still would show up as -1, and would
    still be unable to be a 5, eliminating the duplication problem.