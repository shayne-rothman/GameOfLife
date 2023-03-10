# Game of Life

This program is a simple implementation of mathematician John Conway's Game of Life simulation. Life demonstrates how complex behavior can result from simple rules. At each iteration of the simulation, every cell in the grid is either "alive" or "dead." The cells are updated for the next iteration based on how many of their eight adjacent cells are alive. Furthermore, the grid behaves as a torus, meaning that a cell on the top of the grid's three top neighbors are located on the bottom row of the grid. Similarly, a cell on the far right column of the grid will have neighbors on the far left. For more information on Conway's Game of Life, visit here: https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life

## Compiling and Executing
This repository includes a makefile, so you should be able to compile the program by simply running the `make` command (similarly, you can run `make clean` to remove old versions of executables before recompiling if changes have been made).

The program takes two commandline arguments in order to run: the name of the file containing data for the initial state of the simulation, and an integer indicating whether you wish to see an animation of the simulation (input 1) or simply see how long the simulation takes (input 0). 
For example: `./gol walker.txt 1` will run a simulation using walker.txt and will display an animation of the simulation. 

## Additional Files
Included in this repository are 3 text files with initial states that you can use to run the simulation: 
```
walker.txt
oscillator.txt
grow.txt
```
If you wish to create your own text file to test the simulation, your file should give the desired dimensions of the grid, the number of iterations you'd like the simulation to run, the number of cells that are initially alive, and x-y coordinates for each initially alive cell. For example:  
```
30 (# of rows in the grid)
30 (# of columns)
10 (# of iterations) 
4 (# of cells that are initially alive)
0 15 (coordinates)
21 21
7 30
9 28
```
