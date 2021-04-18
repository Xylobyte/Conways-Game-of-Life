/**
 * @file life.h
 * @author Donovan Griego
 * @brief Function prototypes for gl.c
 * @date 2020-11-29
 */
#include <stdlib.h>
#include <stdio.h>
#ifndef LIFE_H_
#define LIFE_H_

unsigned char **create_matrix(int rows, int cols);
void init_dead(unsigned char **mat, int rows, int cols, char type);
void init_alive(unsigned char **mat, int rows, int cols, char type);
void kill_edges(unsigned char **mat, int rows, int cols);
void print_matrix(unsigned char **mat, int rows, int cols);
void calc_to_life(unsigned char **calc, unsigned char **life, int rows, int cols, char type);
void life_to_calc(unsigned char **life, unsigned char **calc, int rows, int cols, char type);
void process_gen(unsigned char **calc, unsigned char **life, int rows, int cols, char type);
void set_life(unsigned char **life, int row, int col, unsigned char x, char type);
int count_neighbors(unsigned char **calc, int rows, int cols, int row, int col, char type);
void load_file(unsigned char **calc, FILE *fp, int x, int y, char type);
int modulo(int x, int N);
void free_matrix(unsigned char **mat, int rows, int cols);

#endif
