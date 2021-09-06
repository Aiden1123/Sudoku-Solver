#include <stdio.h>
#include <stdlib.h>
#define TRUE 1

//  This programm solves sudoku using brute-force algorithm
//  It is much less effecient than the other version but it still works
//  And actually manages to solve most of the sudokus in acceptable time


int check_row(int key, int row, int column, int (*sudoku)[9]) {     //function that checks if key is already present in the row

    for(int i=0;i<9;i++) {
        
        if (i == column)                //skips the cell in which algorithm tries to input the number
            continue;

        if(sudoku[row][i] == key)
            return 0;                   //returns FALSE if key is already present
    }

    return 1;                           //returns TRUE if key can be placed on this position
}

int check_column(int key, int row, int column, int (*sudoku)[9]) {  //function that checks if key is already present in the column

    for(int i=0;i<9;i++) {

        if (i == row)                   //skips the cell in which algorithm tries to input the number
            continue;
            
        if(sudoku[i][column] == key)
            return 0;                   //returns FALSE if key is already present
    }

    return 1;                           //returns TRUE if key can be placed on this position
}

int check_box(int key, int row, int column, int (*sudoku)[9]) { //function that checks if key is already present in the box
   
    int box_row = row/3;
    int box_column = column/3;
    
    for(int i=0;i<3;i++) {
        for(int j=0;j<3;j++) {
            
            if ((3*box_row)+i==row && ((3*box_column)+j==column))       //skips the cell in which algorithm tries to input the number
                continue;
            
            if (sudoku[(3*box_row)+i][(3*box_column)+j]==key)
                return 0;                                               //returns FALSE if key is already present
            
        }
    }

    return 1;                                                           //returns TRUE if key can be placed on this position
}

int check(int key, int row, int column, int (*sudoku)[9]) {  //function that checks if key can be placed in this position, returns TRUE on success and FALSE otherwise
    return (check_row(key,row,column,sudoku) && check_column(key,row,column,sudoku) && check_box(key,row,column,sudoku));
}

void print_table(int (*ptr)[9]) {       //function that prints the 9x9 table in sudoku-like fashion

    for(int i=0;i<9;i++) {
        for(int j=0;j<9;j++) {
            
            printf("%d ", ptr[i][j]);
            
            if (j%3==2)
                printf("\t");
        }

        printf("\n");
       
        if (i%3==2)
            printf("\n");
    }

    return;
}

int main() {

    //Enter your sudoku here:
    int sudoku[9][9] = {
                        {0,0,0,   8,0,1,   0,0,0},
                        {0,0,0,   0,0,0,   4,3,0},
                        {5,0,0,   0,0,0,   0,0,0},

                        {0,0,0,   0,7,0,   8,0,0},
                        {0,0,0,   0,0,0,   1,0,0},
                        {0,2,0,   0,3,0,   0,0,0},

                        {6,0,0,   0,0,0,   0,7,5},
                        {0,0,3,   4,0,0,   0,0,0},
                        {0,0,0,   2,0,0,   6,0,0},
                    };


    int vars[9][9];     //create vars array
                        //the arrays stores information whether cell can be updated

    for(int i=0;i<9;i++) {
        for(int j=0;j<9;j++) {

            if (sudoku[i][j])
                vars[i][j] = 0;     //FALSE if number cannot be updated

            else
                vars[i][j] = 1;     //TRUE if cell is empty or the number which is there may not belong there
        }
    }

    print_table(sudoku);
    print_table(vars);


   int c_row = 0;       //declaration of needed variables
   int c_col = 0;
   int key;
   int count = 0;
   int er = 0;

    while (c_row < 9) {     //while sudoku is not completed:

        if(count%1000000==0) {
            printf("count: %d\n",count);    //print count every 1 000 000 iteration
        }

        if (!vars[c_row][c_col]) {      //if cell is not to be modified then move to the next cell
            c_col++;            //moves to the next column
            if (c_col == 9) {   //if row is finished it moves to the next row and first column
                c_row++;
                c_col = 0;
            }
            count++;
            continue;
        }
        
        key = sudoku[c_row][c_col]+1;                   //try to enter a valid number 
        while(TRUE) {
            if (check(key, c_row, c_col,sudoku)) {      
                sudoku[c_row][c_col] = key;
                break;
            }
            else                                        //if a number doesn't fit then increase number until it fits
                key++;
        }

        if (sudoku[c_row][c_col] < 10) {                //if entered number is valid then move forward
            c_col++;
            if (c_col == 9) {
                c_row++;
                c_col = 0;
            }
        }
        else {                                          //if not then reset the cell and move back
            sudoku[c_row][c_col] = 0;
           do {
                c_col--;                       //move to a previous columnn
                if (c_col==-1) {               //if row is finished then move to the previous row and last column
                    c_row--;
                    c_col = 8;
                }

                if (c_row == -1) {             //break the loop if not a single solution has been found
                    er = 1;
                    break;
                }

            } while(!vars[c_row][c_col]);      //keep moving until a cell that can be modified is found
        }
        count++;

        if (er) {                              //if not a single solution has been found then close the program
            printf("error index out of range\n");
            break;
        }

    }
    
    print_table(sudoku);
    printf("wykonano: %d iteracji petli\n", count);

    return 0;
}

