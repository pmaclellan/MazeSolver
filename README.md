# MazeSolver
Lab 5 from Optimization Methods (EECE3326).  Uses multiple algorithms to find the shortest path through given mazes.

From the project description:

In this project, you will develop algorithms that find paths through a maze.

The input is a text file containing a collection of mazes. Each maze begins with the number of rows
and columns in the maze and a character for every cell in the maze. A cell contains a space if the
solver is allowed to occupy the cell. A cell contains X if the solver is not allowed to occupy the cell.
The solver starts at cell (0,0) in the upper left, and the goal is to get to cell (rows-1, cols-1)
in the lower right. A legal move from a cell is to move left, right, up, or down to an immediately
adjacent cell that contains a space. Moving off any edge of the board is not allowed.

*Note: Implementation should be separated from .h files and put into .cpp, but there was some issue with the TA's getting linker errors or something so that's why it isn't.
