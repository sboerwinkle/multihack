#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "gui.h"
#include "images.h"
#include "entity.h"

static SDL_Window* window;
SDL_Renderer* render;

static int running = 1;

SDL_Texture **pictures = NULL;

entity **entities;

void drawSprite(int ix, int x, int y) {
	static SDL_Rect r;
	r.x = x*tileSize;
	r.y = y*tileSize;
	r.w = r.h = tileSize;
	SDL_RenderCopy(render, pictures[ix], NULL, &r);
}

void clearScreen() {
	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	SDL_RenderClear(render);
	SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
	int i;
	for (i = 0; i < height; i += tileSize)
		SDL_RenderDrawLine(render, 0, i, width, i);
	for (i = 0; i < width; i += tileSize)
		SDL_RenderDrawLine(render, i, 0, i, height);
}

static void paint(){
	clearScreen();
	//Draw items
	int i;
	for (i = boardSize*boardSize - 1; i >= 0; i--) {
		if (entities[i])
			drawSprite(entities[i]->type, i%boardSize, i/boardSize);
	}
	SDL_RenderPresent(render);
}

static void keyAction(int bit, char pressed){
	/*int i = 0;
	for(; i < numKeys && bit!=keyBindings[i]; i++);
	if(i==numKeys) return;
	keys[i] = pressed;*/
}

int main(int argc, char** argv){
	running = 1;
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	window = SDL_CreateWindow("MultiHack", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
	if(window == NULL){
		fputs("No SDL2 window.\n", stderr);
		fputs(SDL_GetError(), stderr);
		SDL_Quit();
		return 1;
	}
	render = SDL_CreateRenderer(window, -1, 0);

	entities = calloc(boardSize * boardSize, sizeof(entity*));

	entities[boardSize*3 + 2] = malloc(sizeof(entity));
	entities[boardSize*3 + 2]->type = 0;

	loadPics();

	struct timespec t = {.tv_sec = 0};
	struct timespec otherTime, lastTime;
	clock_gettime(CLOCK_MONOTONIC, &lastTime);
	while (running == 1){
		SDL_Event evnt;
		while(SDL_PollEvent(&evnt)){
			if (evnt.type == SDL_QUIT)
				running = -1;
			else if (evnt.type == SDL_KEYDOWN)
				keyAction(evnt.key.keysym.sym, 1);
			else if (evnt.type == SDL_KEYUP)
				keyAction(evnt.key.keysym.sym, 0);
		}
		paint();
		clock_gettime(CLOCK_MONOTONIC, &otherTime);
		t.tv_nsec = 20000000 -
		    (otherTime.tv_nsec - lastTime.tv_nsec +
		    1000000000 * (otherTime.tv_sec - lastTime.tv_sec));
		if (t.tv_nsec > 0) {
			nanosleep(&t, NULL);
		}
		clock_gettime(CLOCK_MONOTONIC, &lastTime);
	}

	free(entities); //TODO: Actually free any remaining entities.
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return running;
}
