/**
 * @file life.c
 * @author Donovan Griego
 * @brief Function definitions for gl.c
 * @details Manipulating matrices used in gl.c
 * @date 2020-11-29
 * @todo none
 * @bug none
 */
#include "life.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Create a matrix object
 * 
 * @param rows number of rows
 * @param cols number of columns
 * @return unsigned char** the address of the created matrix
 */
unsigned char **create_matrix(int rows, int cols) {
    int i;
    int j;
    unsigned char **a;
    a = malloc(rows * sizeof(int *));
    if(!a)
        return NULL; 
    for(i=0;i<rows;i++) {
        a[i] = malloc(cols * sizeof(int));
        if (!a[i]) {
            for (j = 0; j < i; j++)
                free(a[j]);
            free(a);
            return NULL;
        }
    }
    return a;
}

/**
 * @brief initialize a matrix with 0s
 * 
 * @param mat matrix to initialize
 * @param rows number of rows in matrix
 * @param cols number of columns in matrix
 * @param type type of matrix
 */
void init_dead(unsigned char **mat, int rows, int cols, char type) {
    for(int i = 0; i < (type == 'h' ? rows + 2 : rows); i++) {
        for(int j = 0; j < (type == 'h' ? cols + 2 : cols); j++) {
            *(*(mat + i) + j) = 0;
        }
    }
}

/**
 * @brief print matrix in terminal
 * 
 * @param mat matrix to print
 * @param rows number of rows in matrix
 * @param cols number of columns in matrix
 */
void print_matrix(unsigned char **mat, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
}

/**
 * @brief initialize matrix to 1s
 * 
 * @param mat matrix to initialize
 * @param rows number of rows in matrix
 * @param cols number of columns in matrix
 * @param type type of matrix
 */
void init_alive(unsigned char **mat, int rows, int cols, char type) {
    for(int i = 0; i < (type == 'h' ? rows + 2 : rows); i++) {
        for(int j = 0; j < (type == 'h' ? cols + 2 : cols); j++) {
            *(*(mat + i) + j) = 1;
        }
    }
}

/**
 * @brief sets edges of matrix to 0s
 * 
 * @param mat matrix to modify
 * @param rows number of rows in matrix
 * @param cols number of columns in matrix
 */
void kill_edges(unsigned char ** mat, int rows, int cols) {
    rows += 2;
    cols += 2;
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            if(i == rows - 1 || i == 0 || j == cols - 1 || j == 0)
                *(*(mat + i) + j) = 0;
        }
    }
}

/**
 * @brief copies the calculation matrix to the life matrix
 * 
 * @param calc calculation matrix
 * @param life life matrix
 * @param rows number of rows in the matrices
 * @param cols number of columns in the matrices
 * @param type the type of matrix calc is based on current mode
 */
void calc_to_life(unsigned char **calc, unsigned char **life, int rows, int cols, char type) {
    if(type != 'h')
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                *(*(life + i) + j) = *(*(calc + i) + j);
            }
        }
    if(type == 'h')
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                *(*(life + i) + j) = *(*(calc + i + 1) + j + 1);
            }
        }
}

/**
 * @brief copies the life matrix to the calculation matrix
 * 
 * @param life life matrix
 * @param calc calculation matrix
 * @param rows number of rows in the matrices
 * @param cols number of columns in the matrices
 * @param type the type of matrix calc is based on current mode
 */
void life_to_calc(unsigned char **life, unsigned char **calc, int rows, int cols, char type) {
    if(type == 'h') {
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                *(*(calc + i + 1) + j + 1) = *(*(life + i) + j);
            }
        }
    } else {
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                *(*(calc + i) + j) = *(*(life + i) + j);
            }
        }
    }
}

/**
 * @brief Sets a value from a given coordinate in life
 * 
 * @param life life matrix
 * @param row row of the 1
 * @param col column of the 1
 * @param x value to set at coordinate
 * @param type type of matrix the coordinates are from (calc matrix type)
 */
void set_life(unsigned char **life, int row, int col, unsigned char x, char type) {
    if(type != 'h')
        *(*(life + row) + col) = x;
    if(type == 'h')
        *(*(life + row - 1) + col - 1) = x;
}

/**
 * @brief Processes the next generation from calc and saves the chagnes in life
 * 
 * @param calc calculation matrix
 * @param life life matrix
 * @param rows number of rows in matrices
 * @param cols number of columns in matrices
 * @param type type of matrix calc is
 */
void process_gen(unsigned char **calc, unsigned char **life, int rows, int cols, char type) {
    if(type == 'h') {
        kill_edges(calc, rows, cols);
        for(int i = 1; i <= rows; i++) {
            for(int j = 1; j <= cols; j++){
                unsigned char curr = *(*(calc + i) + j);
                int neighbors = count_neighbors(calc, rows, cols, i, j, type);
                if(neighbors < 2 && curr == 1)
                    set_life(life, i, j, 0, type);
                if(neighbors > 3 && curr == 1)
                    set_life(life, i, j, 0, type);
                if((neighbors == 3 || neighbors == 2) && curr == 1)
                    set_life(life, i, j, 1, type);
                if(neighbors == 3 && curr == 0)
                    set_life(life, i, j, 1, type);
            }
        }
    } else {
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++){
                unsigned char curr = *(*(calc + i) + j);
                int neighbors = count_neighbors(calc, rows, cols, i, j, type);
                if(neighbors < 2 && curr == 1)
                    set_life(life, i, j, 0, type);
                if(neighbors > 3 && curr == 1)
                    set_life(life, i, j, 0, type);
                if((neighbors == 3 || neighbors == 2) && curr == 1)
                    set_life(life, i, j, 1, type);
                if(neighbors == 3 && curr == 0)
                    set_life(life, i, j, 1, type);
            }
        }
    }
}

/**
 * @brief Counts the neighbors of a given cell based on current mode
 * 
 * @param calc calculation matrix
 * @param rows total number of rows
 * @param cols total number of columns
 * @param row row of cell to check
 * @param col column of cell to check
 * @param type type of matrix calc is and how to process neighbors
 * @return int number of neighbors a cell has
 */
int count_neighbors(unsigned char **calc, int rows, int cols, int row, int col, char type) {
    int count = 0;
    if(type == 'h') {
        for(int i = -1; i <= 1; i++) {
            for(int j = -1; j <= 1; j++) {
                if(*(*(calc + row + i) + col + j) == 1 && !(i == 0 && j == 0)) {
                    count++;
                }
            }
        }
    }
    if(type == 't') {
        for(int i = -1; i <= 1; i++) {
            for(int j = -1; j <= 1; j++) {
                if(*(*(calc + modulo(row + i, rows)) + modulo(col + j, cols)) == 1 && !(i == 0 && j == 0)) {
                    count++;
                }
            }
        }
    }
    if(type == 'k') {
        for(int i = -1; i <= 1; i++) {
            for(int j = -1; j <= 1; j++) {
                
                if(row + i == -1 || row + i == rows) {
                    if(col + j < cols / 2){
                        if(*(*(calc + (modulo(row + i, rows))) + ((cols-1) - modulo(col + j, cols))) == 1 && !(i == 0 && j == 0)) {
                            count++;
                        }
                    } else if(col + j >= cols / 2){
                        if(*(*(calc + (modulo(row + i, rows))) + ((cols-1) - modulo(col + j, cols))) == 1 && !(i == 0 && j == 0)) {
                            count++;
                        }
                    }
                }else
                if(*(*(calc + modulo(row + i, rows)) + (modulo(col + j, cols))) == 1 && !(i == 0 && j == 0)) {
                    count++;
                }
            }
            //printf("\n");
        }
    }
    //printf("\n\n");
    return count;
}

/**
 * @brief loads a structure from a 1.06 life file into calculation matrix
 * 
 * @param calc calculation matrix
 * @param fp file stream
 * @param x row to load structure
 * @param y column to load structure
 * @param type type of matrix calc is
 */
void load_file(unsigned char **calc, FILE *fp, int x, int y, char type) {
    int tmp_x;
    int tmp_y;
    char in[25];
    
    while(fgets(in, 25, fp)){
        sscanf(in, "%d %d", &tmp_x, &tmp_y);
        if(type == 'h')
            *(*(calc + x + tmp_x + 1) + y + tmp_y + 1) = 1;
        if(type != 'h')
            *(*(calc + x + tmp_x) + y + tmp_y) = 1;
    }
}

/**
 * @brief Calculates the modulo of two numbers
 * 
 * @param x number to calculate
 * @param N mod number
 * @return int result
 */
int modulo(int x, int N) {
    return (x % N + N) % N;
}

/**
 * @brief frees the memory held by the passed in matrix
 * 
 * @param mat matrix to free
 * @param rows number of rows in the matrix
 * @param cols number of columns in the matrix
 */
void free_matrix(unsigned char **mat, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        free(mat[i]);
    }
    free(mat);
}