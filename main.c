#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0
#define PRINT_SEPARATOR printf("\n////////////////////\n\n");


// This program tries to solve Sudoku using several methods
// Firstly it attempts to find patterns that imply only one solution to the partial problem
// Either it searches for cells with a single option or numbers that can be placed only in one position
// After it finds and solves all such patterns it moves on to the greedy algorithm
// Program tries to fill a sudoku with valid numbers, it moves on until it finds an error or solves entire sudoku
// In case of an error program moves back to previous cells and updates them and then moves on again until the sudoku is solved
// The key to making this more efficient is to start with cells that have fewest options
// If we assume that each cell has only one correct number then fewer options mean more probable guess
// That's why the algorithm is more efficcient than simple brute-force algorithm


typedef struct coordinates {        //cords structure used for greedy algorithm
    int row;
    int column;
} cords;

void print_queue(cords *q, int size) {
    for(int i=0;i<size;i++) {
        printf("(%d,%d) ",q[i].row,q[i].column);
    }
    printf("\n");
    return;
}

int *options[9][9];                 //global pointer to options matrix
                                    //holds information about possible options for each cell

int empty_fields_count(int (*array)[9]) {   //function that counts all empty cells for greedy algorithm
                                            //in vars matrix each cell is marked as 1 if empty and 0 otherwise  
                                            //so the number of empty fields is equal to sum of vars matrix
    int sum=0;
    for(int i=0;i<9;i++) {
        for(int j=0;j<9;j++) {
            sum+=array[i][j];
        }
    }
    return sum;
}

void clear_options(int *array) {    //function to delete all options used while entering new number
    for(int i=0;i<9;i++) 
        array[i]=0;
    return;
}

int options_count(int *array) {     //function to count number of options
    int sum = 0;
    for(int i=0;i<9;i++)
        sum+=array[i];
    return sum;
}

int number_with_one_place_in_row(int *row[9]) {     //function that checks if a row has only one place for a number
                                                    //returns the number on success or 0 if such number has not been found
    
    int sum[9] = {0,0,0,0,0,0,0,0,0};       //array which holds information about how many times each number can be placed in the row
    
    for(int i=0;i<9;i++) {                  //filling the array
        for(int j=0;j<9;j++) {
            sum[j] += row[i][j];            
        }
    }

    for(int i=0;i<9;i++) {                  //searching array for number with only one possible location
        if (sum[i]==1) 
            return i+1;
    }

    return 0;
}

int number_with_one_place_in_column(int column) {   //function that checks if a column has only one place for a number
                                                    //returns the number on success or 0 if such number has not been found
    
    int sum[9] = {0,0,0,0,0,0,0,0,0};   //array which holds information about how many times each number can be placed in the column
    
    for(int i=0;i<9;i++) {              //filling the array
        for(int j=0;j<9;j++) {
            sum[j] += options[i][column][j];
        }
    }

    for(int i=0;i<9;i++) {              //searching array for number with only one possible location
        if (sum[i]==1) 
            return i+1;
    }

    return 0;
}

int number_with_one_place_in_box(int box_row,int box_column) {  //function that checks if a box has only one place for a number
                                                                //returns the number on success or 0 if such number has not been found
    
    int sum[9] = {0,0,0,0,0,0,0,0,0};       //array which holds information about how many times each number can be placed in the box
    
    for(int i=box_row*3;i<(box_row+1)*3;i++) {      //filling the array
        for(int j=box_column*3;j<(box_column+1)*3;j++) {
            for(int k=0;k<9;k++) {
                sum[k] += options[i][j][k];
            }
        }
    }

    for(int i=0;i<9;i++) {      //searching array for number with only one possible location
        if (sum[i]==1) 
            return i+1;
    }

    return 0;
}

int get_option(int *array) {    //function that returns the number that can be entered in the cell
    for(int i=0;i<9;i++) {
        if (array[i])
            return i+1;
    }
}

int compare_cords(const void *one, const void *two) {       //function for coordinates which compares possible options

    cords *a = (cords *) one;
    cords *b = (cords *) two;

    int options_a = options_count(options[a->row][a->column]);
    int options_b = options_count(options[b->row][b->column]);

    return options_a-options_b;
}

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

int is_finished(int (*vars)[9]) {               //function that checks if sudoku is completed before greedy algorithm begins
    
    for(int i=0;i<9;i++) {
        for (int j=0;j<9;j++) {

            if (vars[i][j]) {                   //if there is a cell which needs to be filled returns FALSE
                return FALSE;
            }

        }
    }

    return TRUE;                                //returns TRUE if sudoku is solved
}

int main() {

    for(int i=0;i<9;i++) {                              //memory allocation for options array
        for (int j=0;j<9;j++) {
            options[i][j] = calloc(9,sizeof(int));
        }
    }


                        //Enter your sudoku below:
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

    int key;
    int new_changes = FALSE;   //flag used to reset the loop if sudoku has changed


    //1st stage: trying to fill as many cells as possible using smart methods


    do {

        if (is_finished(vars)) {    //quit the loop if sudoku is completely filled
            break;
        }

        new_changes = FALSE;

        for(int i=0;i<9;i++) {                              //update options matrix
            for (int j=0;j<9;j++) {
            
                if (vars[i][j]==FALSE)                      //if cell is filled set all options to FALSE
                    continue;
            
                for (int k=0;k<9;k++) {
                    
                    if (check(k+1,i,j,sudoku))              //for each cell each number is going to be set TRUE if it can be placed there or FALSE otherwise
                        options[i][j][k]=TRUE;

                    else
                        options[i][j][k]=FALSE;
                }
            }
        }

        //single option method:
        for(int i=0;i<9;i++) {                                  
            for (int j=0;j<9;j++) {
            
                if (vars[i][j]==FALSE)    //skip filled cells
                    continue;

                if (options_count(options[i][j])==1) {      //if only one number can be placed in the cell

                    printf("single option method: sudoku[%d][%d] = %d\n",i,j,get_option(options[i][j]));
                    sudoku[i][j] = get_option(options[i][j]);   //fill the cell with the number
                    vars[i][j] = FALSE;                         //make sure it is not going to be changed
                    new_changes = TRUE;                         //raise the flag to reset the loop
                    clear_options(options[i][j]);               //set all options to FALSE for this cell
                    break;                                      //reset the loop
                }
            }
            if (new_changes)
                break;
        }

        if (new_changes)
            continue;

        //single cell in a row:
        for(int i=0;i<9;i++) {                                  //for each row:
        
            key = number_with_one_place_in_row(options[i]);     //check if there is a number that can be placed only in one cell

            if (key==0)                                         //continue if not
                continue;

            else {                                              //if there is then find the cell for it and fill it
                for(int j=0;j<9;j++) {
                    if (check(key,i,j,sudoku)) {
                    
                        if (vars[i][j]==FALSE) {
                            continue;
                        }

                        printf("single cell in a row: sudoku[%d][%d] = %d\n",i,j,key);      
                        sudoku[i][j] = key;             //fills the cell
                        vars[i][j] = FALSE;             //make sure it is not going to be changed
                        new_changes=TRUE;               //raise the flag to reset the loop
                        clear_options(options[i][j]);   //set all options to FALSE for this cell
                        break;                          //reset the loop
                    }
                }
            }
            if (new_changes)
                break;
        }
        if (new_changes)
            continue;

        //single cell in a column
        for(int j=0;j<9;j++) {      //for each column:
        
            key = number_with_one_place_in_column(j);   //check if there is a number that can be placed only in one cell

            if (key==0)                                 //continue if not
                continue;   

            else {                                      //if there is then find the cell for it and fill it
                for(int i=0;i<9;i++) {
                    if (check(key,i,j,sudoku)) {
                    
                        if (vars[i][j]==FALSE) {
                            continue;
                        }
                        printf("single cell in a column: sudoku[%d][%d] = %d\n",i,j,key);
                        sudoku[i][j] = key;             //fills the cell
                        vars[i][j] = FALSE;             //make sure it is not going to be changed
                        new_changes=TRUE;               //raise the flag to reset the loop
                        clear_options(options[i][j]);   //set all options to FALSE for this cell
                        break;                          //reset the loop
                    }
                }
            }
            if (new_changes)
                break;
        }
        if (new_changes)
            continue;

        //single cell in a box
        for(int box_x=0;box_x<3;box_x++) {                             //for each box:
            for(int box_y=0;box_y<3;box_y++) {

                key = number_with_one_place_in_box(box_x,box_y);        //check if there is a number that can be placed only in one cell

                if (key==0)                                             //continue if not
                    continue;

                for(int i=box_x*3;i<(box_x+1)*3;i++) {                  //if there is then find the cell for it and fill it
                    for(int j=box_y*3;j<(box_y+1)*3;j++) {
                    
                        if (check(key,i,j,sudoku)) {
                       
                            if (vars[i][j]==FALSE) {
                                continue;
                            }

                            printf("single cell in a box: sudoku[%d][%d] = %d\n",i,j,key);
                            sudoku[i][j] = key;             //fills the cell
                            vars[i][j] = FALSE;             //make sure it is not going to be changed
                            new_changes=TRUE;               //raise the flag to reset the loop
                            clear_options(options[i][j]);   //set all options to FALSE for this cell
                            break;                          //reset the loop
                        }
                    }
                if (new_changes)
                    break;
                }
            if (new_changes)
                break;
            }
        if (new_changes)
            break;
        }

    } while(new_changes);   //If nothing changed exit the loop




    PRINT_SEPARATOR
    print_table(sudoku);
    print_table(vars);

    if (is_finished(vars)) {
        printf("Sudoku solved succesfully\n");
        
        //free options array
        for(int i=0;i<9;i++) {
            for (int j=0;j<9;j++) {
                free(options[i][j]);
            }
        }
        
        
        return 0;
        
    }

    //if sudoku isn't filled yet the greedy algorithm is used to fill remaining cells
    printf("Greedy algorithm start\n");


    int fields_count = empty_fields_count(vars);            //count how many cells are not filled
    cords* queue = malloc(fields_count*sizeof(cords));      //create a queue of coordinates
    int current_fields_count=0;


    for(int i=0;i<9;i++) {                                  //fill the queue
        for(int j=0;j<9;j++) {

            if(vars[i][j]) {
                queue[current_fields_count].column = j;
                queue[current_fields_count].row = i;
                current_fields_count++;
            }

        }
    }

    print_queue(queue,fields_count);
    qsort(queue, fields_count, sizeof(cords), compare_cords);  //sort cells in ascending order based on amount of possible options
    print_queue(queue,fields_count);

    int iterator = 0;
    int count = 0;
    int c_row = 0;
    int c_col = 0;

    while(iterator<fields_count) {    //while queue is not empty:         
        
        if(count%10000==0) {                    
            printf("count: %d\n",count);    //print count every 10 000 times
        }

        if(iterator<0) {                    //make sure iterator did not leave the queue
            printf("error: iterator out of range\n");
            print_table(sudoku);
            return 0;
        }
        
        c_row = queue[iterator].row;        //row of the current cell
        c_col = queue[iterator].column;     //column of the current cell

        for(int i=sudoku[c_row][c_col];i<=10;i++) {     //increase the number in cell until it fits the criteria
            if (i==sudoku[c_row][c_col])    
                i += 1;
            if(check(i,c_row,c_col,sudoku)) {
                sudoku[c_row][c_col]=i;
                break;
            }
        }

        count++;

        if(sudoku[c_row][c_col]==10) {      //if the number entered is 10 then reset cell back to 0 and move to previous cell
            sudoku[c_row][c_col]=0;
            iterator--;
        }
        else {
            iterator++;                     //if the number is valid move on to the next cell
        }
    }

    PRINT_SEPARATOR
    puts("The solution:\n");
    print_table(sudoku);
    print_table(vars);
    
    printf("sudoku solved after %d loop iterations\n",count);
    
    free(queue);

    //free options array
    for(int i=0;i<9;i++) {
        for (int j=0;j<9;j++) {
            free(options[i][j]);
        }
    }
    
    return 0;
    
}