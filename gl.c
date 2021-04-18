/**
 * @file gl.c
 * @author Donovan Griego
 * @brief Game of life with 3 modes: hedge, torus, and klein
 * @details Uses source in life.c to play the game of life
 * @date 2020-11-29
 * @todo none
 * @bug none
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "SDL2/SDL.h" 
#include "sdl.h"
#include "life.h"

int main(int argc, char *argv[])
{
	int width = 800;
	int height = 600;
	int sprite_size = 4;
	int rows;
	int cols;
	int c;
	int origin_x;
	int origin_y;
	int tick_speed = 1;
	unsigned char red = 140;
	unsigned char green = 145;
	unsigned char blue = 250;
	unsigned char **life;
	unsigned char **calc;
	char type;
	char file_type[25];
	FILE *fp = NULL;
    struct sdl_info_t sdl_info;

	if (argc == 1) {
		printf("usage: life -f filename -w width -h height -e edge -r red -g green -b blue -s size -o origin -t tick_speed\n");
		exit(EXIT_FAILURE);
	}
	while((c = getopt(argc, argv, ":w:h:e:r:g:b:s:f:o:t:H")) != -1) {
		switch(c) {
		case 'w':
			width = atoi(optarg);
 			break;
		case 'h':
			height = atoi(optarg);
			break;
		case 'e':
			if(strcmp(optarg, "hedge") == 0)
				type = 'h';
			else if(strcmp(optarg, "torus") == 0)
				type = 't';
			else if(strcmp(optarg, "klein") == 0)
				type = 'k';
			else {
				printf("Error; '%s' is not a valid option for edge\n", optarg);
				return 1;
			}
			break;
		case 'r':
			red = atoi(optarg);
			break;
		case 'g':
			green = atoi(optarg);
			break;
		case 'b':
			blue = atoi(optarg);
			break;
		case 's':
			if(atoi(optarg) == 2)
				sprite_size = atoi(optarg);
			else if(atoi(optarg) == 4)
				sprite_size = atoi(optarg);
			else if(atoi(optarg) == 8)
				sprite_size = atoi(optarg);
			else if(atoi(optarg) == 16)
				sprite_size = atoi(optarg);
			else
				sprite_size = 0;
			break;
		case 'f':
			errno = 0;
			fp = fopen(optarg, "r");
			if (fp == NULL) {
				fprintf(stderr, "%s: argument to option '-f' failed: %s\n", argv[0], strerror(errno));
				exit(EXIT_FAILURE);
			}
			fgets(file_type, 11, fp);
			if(strcmp(file_type, "#Life 1.06") != 0){
				printf("Invalid file format; Please use 1.06 format\n");
				exit(EXIT_FAILURE);
			}
			fgets(file_type, 25, fp);	//gets rid of new line characters cut off previously
			break;
		case 'o':
			sscanf(optarg, "%d,%d", &origin_x, &origin_y);
			break;
		case 't':
			tick_speed = atoi(optarg);
			break;
		case 'H':
			printf("usage: life -f filename -w width -h height -e edge -r red -g green -b blue -s size -o origin -t tick_speed\n");
			printf("filename: filename, a life pattern in file format 1.06\n");
			printf("width: width of the screen argument 640, 800, 1024, etc.\n");
			printf("height: height of the screen argument 480, 600, 768, etc.\n");
			printf("edge: type of edge. Values are hedge, torus, or klein (strings)\n");
			printf("red: the red color value, an integer between [0, 255]\n");
			printf("green: the green color value, an integer between [0, 255]\n");
			printf("blue: the blue color value, an integer between [0, 255]\n");
			printf("size: size of the sprite. Valid values are 2, 4, 8, or 16 only. An integer.\n");
			printf("origin: x,y the initial x,y coordinate of the pattern found in the file. No space between the x and y.\n");
			printf("tick_speed: the speed at which the generations run. Higher is slower.\n");
			exit(EXIT_SUCCESS);
		case ':':
			fprintf(stderr, "%s: option '-%c' requires an argument\n", argv[0], optopt);
			exit(EXIT_FAILURE);
		case '?':
		default:
			printf("illegal option %c - ignored\n", optopt);
			printf("usage: life -f filename -w width -h height -e edge -r red -g green -b blue -s size -o origin -t tick_speed\n");
			break;
				
		}
	}
	init_sdl_info(&sdl_info, width, height, sprite_size, red, green, blue);
	cols = width/sprite_size;
	rows = height/sprite_size;


	life = create_matrix(rows, cols);
	calc = create_matrix(type == 'h' ? height/sprite_size + 2 : rows, type == 'h' ? width/sprite_size + 2 : cols);

	init_dead(calc, rows, cols, type);
	load_file(calc, fp, origin_x, origin_y, type);
	fclose(fp);
	calc_to_life(calc, life, rows, cols, type);

	while (1)
	{
		 if (SDL_GetTicks() % tick_speed == 0) {
			sdl_render_life(&sdl_info, life);			//life matrix is the only one passed into sdl
			process_gen(calc, life, rows, cols, type);	//processing is done using calc matrix and changes are made to life matrix
			life_to_calc(life, calc, rows, cols, type);	//new changes are copied from life matrix to calc for next generation
		 }

		SDL_Event event;
		while (SDL_PollEvent(&event)) 
		{
			switch (event.type) 
			{
			case SDL_KEYDOWN:
				break;
			case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					free_matrix(calc, type == 'h' ? rows + 2 : rows, type == 'h' ? cols + 2 : cols);
					free_matrix(life, rows, cols);
					return 0;
				}
				break;
			case SDL_QUIT:
				free_matrix(calc, type == 'h' ? rows + 2 : rows, type == 'h' ? cols + 2 : cols);
				free_matrix(life, rows, cols);
				return 0;
			}
		}
	}
	free_matrix(calc, type == 'h' ? rows + 2 : rows, type == 'h' ? cols + 2 : cols);
	free_matrix(life, rows, cols);
	return 0;
}
