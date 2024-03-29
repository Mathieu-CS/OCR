#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include "solver.h"

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
// N is the size of the 2D matrix   N*N
#define N 9

char sudoku[81];


/* A utility function to print grid */
void print(int arr[N][N])
{
    printf("---------------------\n");
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d ",arr[i][j]);

            if (j == 2 || j == 5)
            {
                printf("| ");
            }
        }

        printf("\n");
        
        if (i == 2 || i == 5)
        {
            printf("---------------------\n");
        }
        
    }
    printf("---------------------\n");
}
 
// Checks whether it will be legal 
// to assign num to the
// given row, col
int isSafe(int grid[N][N], int row,
                       int col, int num)
{
     
    // Check if we find the same num
    // in the similar row , we return 0
    for (int x = 0; x <= 8; x++)
        if (grid[row][x] == num)
            return 0;
 
    // Check if we find the same num in the
    // similar column , we return 0
    for (int x = 0; x <= 8; x++)
        if (grid[x][col] == num)
            return 0;
 
    // Check if we find the same num in the
    // particular 3*3 matrix, we return 0
    int startRow = row - row % 3,
                 startCol = col - col % 3;
   
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grid[i + startRow][j +
                          startCol] == num)
                return 0;
 
    return 1;
}
 
/* Takes a partially filled-in grid and attempts
to assign values to all unassigned locations in
such a way to meet the requirements for
Sudoku solution (non-duplication across rows,
columns, and boxes) */
int solveSuduko(int grid[N][N], int row, int col)
{
     
    // Check if we have reached the 8th row
    // and 9th column (0
    // indexed matrix) , we are
    // returning true to avoid
    // further backtracking
    if (row == N - 1 && col == N)
        return 1;
 
    //  Check if column value  becomes 9 ,
    //  we move to next row and
    //  column start from 0
    if (col == N)
    {
        row++;
        col = 0;
    }
   
    // Check if the current position
    // of the grid already contains
    // value >0, we iterate for next column
    if (grid[row][col] > 0)
        return solveSuduko(grid, row, col + 1);
 
    for (int num = 1; num <= N; num++)
    {
         
        // Check if it is safe to place
        // the num (1-9)  in the
        // given row ,col  ->we move to next column
        if (isSafe(grid, row, col, num)==1)
        {
            /* assigning the num in the
               current (row,col)
               position of the grid
               and assuming our assigned num
               in the position
               is correct     */
            grid[row][col] = num;
           
            //  Checking for next possibility with next
            //  column
            if (solveSuduko(grid, row, col + 1)==1)
                return 1;
        }
       
        // Removing the assigned num ,
        // since our assumption
        // was wrong , and we go for next
        // assumption with
        // diff num value
        grid[row][col] = 0;
    }
    return 0;
}

char* sudokuStr(int grid[N][N])
{
	int count = 0;
        char* sudoku = malloc(111 * sizeof(char));
	sudoku[110] = '\0';
	for (int i = 0; i < N && count < 110; i++)
	{
            for (int j = 0; j < N && count < 110; j++)
            {


                if (grid[i][j] == 0)
                {
                  
                    sudoku[count] = '.';

                }

           
                else
                {
                    sudoku[count] = grid[i][j] + '0';
                }
                printf("%i\n", count);
                printf("%c\n", sudoku[count]);
                count++;

                if (j == 2 || j == 5)
                {
                    sudoku[count] = ' ';
                    count++;
                }
            
                if (j == 8)
                {
                    sudoku[count] = '\n';
                    count++;
                }
                
                if ((i == 2 || i == 5) && j == 8)
                {
                    sudoku[count] = '\n';
                    count++;
                }
            }
            if (i == 8)
                return sudoku;
	}
        printf("here");
        return sudoku;
}
