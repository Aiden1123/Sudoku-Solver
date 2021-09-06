# Sudoku-Solver

So I came across a sudoku puzzle one day and thought to myself why should I solve a single sudoku if I can create an algorithm and solve all sudokus. Hence this program was made. I created it using the best programming language there is -  the C language.

The program can be found in main.c file. It contains a sample sudoku and the algorithm that I used in this project. Basically the program will firstly look for the cells that can be filled only with a single number and then fill them. Then it is going to look for a row/column/box in which there is only one possible place for a number that isn't present there yet. The program will continue to fill as many numbers as possible using this methods however not all puzzles can be solved this way so if it isn't sufficient it will move on to the greedy algorithm.
This algorith is pretty similair to the simple brute-force algorithm (which was my first attempt to write a sudoku-solver and you can find it in this project) however it fills the cells starting from those with fewest options. That's because if we assume that each cell can be filled with only one number then fewer options mean it is more probable that the guess is correct. It sounds ridiculous but it actually works.

I have also uploaded some sudokus for you to try by simply copy-pasting
