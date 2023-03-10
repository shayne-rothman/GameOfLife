/*
 * Shayne Rothman, Swarthmore College CS31 Lab 7
 */

/*
 * This program displays a simulation of Conway's Game of life, given a few
 text files that include how many iterations of the simulation that should be
 completed and how many cells should begin the simulation alive and their
 locations. If the user selects 1 when starting the program, the simulation
 will be shown - if they input 0 then the program will just print out how long
 it takes to run the simulation.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

void initMatrix(int *matrix1, int *matrix2, int rows, int cols, int init_alive, FILE *file_ptr);
void set(int *matrix, int i, int j, int cols, int val);
int get(int *matrix, int i, int j, int cols);
void printBoard(int *matrix, int rows, int cols);
void updateBoard(int *matrix_read, int *matrix_write, int rows, int cols);
int NumAliveFriends(int *matrix, int i, int j, int rows, int cols);
void run_simulation(int* matrix1, int* matrix2, int iters, int rows, int cols, int choice);
void getCurrentTime(struct timeval *t);
void printElapsedTime(struct timeval start, struct timeval stop, int iters, int rows, int cols);


/*
*This function sets up our two matrices which store all the data for our boards
*/

void initMatrix(int *matrix1, int *matrix2, int rows, int cols, int init_alive,
FILE *file_ptr){
  int ret, x, y;
  for(int i=0; i<rows; i++){
    for(int j=0; j<cols; j++){
      matrix1[i*rows + j] = 0; //init everything as dead
      matrix2[i*rows + j] = 0;
    }
  }
  //adds the initial alive cells to matrix1
  for (int i=0; i<init_alive; i++){
    ret = fscanf(file_ptr, "%d %d", &y, &x); //get x and y coords of alive cells
    if (ret != 2) {
       printf("Invalid input file.\n");
       exit(1);
    }
    set(matrix1, y, x, cols, 1);
  }
}

/*
*This function sets the value at a given index, [i][j], to a given value, val
*/
void set(int *matrix, int i, int j, int cols, int val) {
  matrix[i*cols + j] = val;
}

/*
* This function returns the value at a given index([i][j]) of a matrix
*/
int get(int *matrix, int i, int j, int cols) {
  return matrix[i*cols + j];
}

/*
* this function prints out the matrix into a board of cells as well as the number
* of alive cells on the board at that time
*/
void printBoard(int *matrix, int rows, int cols){
  int i, j, num_alive, val;
  num_alive = 0;
  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      if (matrix[i*cols +j]){
        num_alive +=1;
      }
      val = get(matrix, i, j, cols);
      if (val){
        printf("%s", "\e[1;36m @ \e[0m");
      }
      else{
        printf("%s", "\e[1;35m - \e[0m");
      }
    }
    printf("\n");
  }
  printf("number of live cells: %d\n", num_alive);
}

/*
* This function finds all of the neighbors of a given cell, accounting for
* situations where the neighbor's position is wrapped around the torus grid, and
* returns the number neighbors that are alive
*/
int NumAliveFriends(int *matrix, int i, int j, int rows, int cols){
  int above, below, left, right;
  int top, topleft, lft, botleft, bottom, botright, rgt, topright;
  int counter = 0;
  above = i-1;
  //alternate versions could simplify this logic using the modulo (%) operator
  if (above <0){ //if y-coord is top of grid top neighbors are at bottom of grid
    above = (rows-1);
  }
  below = i +1;
  if (below > (rows-1)){ //vice versa
    below = 0;
  }
  right = j+1;
  if (right > (cols-1)){ //if x-coord is far right, wrap right neighbors to leftmost column
    right = 0;
  }
  left = j-1;
  if (left<0){ //vice versa
    left = (cols-1);
  }

  top = get(matrix, above, j, cols);
  topleft = get(matrix, above, left, cols);
  topright = get(matrix, above, right, cols);
  lft = get(matrix, i, left, cols);
  botleft = get(matrix, below, left, cols);
  bottom = get(matrix, below, j, cols);
  botright = get(matrix, below, right, cols);
  rgt = get(matrix, i, right, cols);

  counter += top;
  counter += topleft;
  counter += topright;
  counter += lft;
  counter += rgt;
  counter += botright;
  counter += bottom;
  counter += botleft;

  return counter;
}
/*
 * This function will update the board by reading from one matrix and
 * writing the required changes based on Conway's game of life to the
 * second matrix:
 * 1. An alive cell with zero or one neighbors dies of loneliness.
 * 2. An alive cell with four or more neighbors dies from overpopulation.
 * 3. An alive cell with two or three neighbors stays alive.
 * 4. A dead cell with exactly three neighbors becomes alive.
 */
 void updateBoard(int *matrix_read, int *matrix_write, int rows, int cols){
   int num, val;
   for (int i=0; i<rows; i++){
     for (int j = 0; j<cols; j++){
       num = NumAliveFriends(matrix_read, i, j, rows, cols);
       val = get(matrix_read, i, j, cols);
       if (val){ //if current cell is alive
         if (num<=1){ //kill cells that need to die
           set(matrix_write, i, j, cols, 0);
         }
         else if (num>=4){
           set(matrix_write, i, j, cols, 0);
         }
         else{ //stay alive
           set(matrix_write, i, j, cols, 1);
         }
       }
       else { //if current cell is dead
         if (num==3){ //reanimate
           set(matrix_write, i, j, cols, 1);
         }
         else{ //stay dead
           set(matrix_write, i, j, cols, 0);
         }
       }
     }
   }
}
/*
 * This function will run the simulation, and display the board if the user
 * requests to show the simulation
 */
void run_simulation(int* matrix1, int* matrix2, int iters, int rows, int cols, int choice){
  if (choice){
    printBoard(matrix1, rows, cols);
    usleep(300000);
    system("clear");
    }
  for (int i=0; i<iters; i++){
    if (i%2==0){//if we are on an even iteration
      updateBoard(matrix1, matrix2, rows, cols);
      if (choice){
        printBoard(matrix2, rows, cols);
        usleep(300000);
        system("clear");
      }
    }
    else { //switch read and write matrix each iteration
      updateBoard(matrix2, matrix1, rows, cols);
      if (choice){
        printBoard(matrix1, rows, cols);
        usleep(300000);
        system("clear");
      }
    }
  }
}
/*
This function gets the current time of day
*/
void getCurrentTime(struct timeval *t) {
  int ret;
  ret = gettimeofday(t, NULL);
  if (ret != 0) {
    printf("Failed to get current time.\n");
    exit(1);
  }
}
/*
This function prints the elapsed time between the start and stop time of the simulation
*/
void printElapsedTime(struct timeval start, struct timeval stop, int iters, int rows, int cols) {
  double t1, t2;
  t1 = start.tv_sec + start.tv_usec/1000000.0;
  t2 = stop.tv_sec + stop.tv_usec/1000000.0;
  printf("%d iterations of a %dx%d board took %f seconds\n", iters, rows, cols, t2-t1);
}

/*
 * The main function reads in user arguments, completes basic error handling,
 * and starts the simulation depending on user choices
 * e.x. which text file to use or toggling showing the simulation
 */
int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("usage: ./gol inputfile.txt operation\n");
    exit(1);
  }

  FILE *file_ptr = NULL;
  int rows, cols, iters, init_alive, ret;
  int *matrix1 = NULL;
  int *matrix2 = NULL;
  int choice = atoi(argv[2]);
  struct timeval start_time, stop_time;

  file_ptr = fopen(argv[1], "r");
  if (file_ptr == NULL) {
    printf("Error: could not open file %s\n.", argv[1]);
    exit(1);
  }

  if ((choice < 0) || (choice>1)){
    printf("Please select 1 or 0 for toggling animation.\n");
    exit(1);
  }


//attempt to read file; get #rows, #columns, #iterations, #initally alive cells
  ret = fscanf(file_ptr, "%d %d %d %d", &rows, &cols, &iters, &init_alive);
  if (ret != 4) {
     printf("Invalid input file.\n");
     exit(1);
   }
//matrices are technically 1d arrays for the sake of memory efficiency,
//but will be treated as 2d arrays
  matrix1 = malloc(sizeof(int)*rows*cols);
  if (!matrix1) {
    printf("Malloc 1 failed.\n");
    exit(1);
  }
  matrix2 = malloc(sizeof(int)*rows*cols);
  if (!matrix2) {
    printf("Malloc 2 failed.\n");
    exit(1);
  }

//initialize everything and run simulation!
  initMatrix(matrix1, matrix2, rows, cols, init_alive, file_ptr);
  getCurrentTime(&start_time);
  run_simulation(matrix1, matrix2, iters, rows, cols, choice);
  getCurrentTime(&stop_time);
  printElapsedTime(start_time, stop_time, iters, rows, cols);

//clean up memory
  free(matrix1);
  free(matrix2);


  return 0;
}
