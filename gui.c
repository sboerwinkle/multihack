#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "gui.h"
#include "images.h"

static SDL_Window* window;
SDL_Renderer* render;

static int running = 1;

SDL_Texture **pictures = NULL;

void drawSprite(int ix, int x, int y) {
	static SDL_Rect r;
	r.x = x*tileSize;
	r.y = y*tileSize;
	r.w = r.h = tileSize;
	SDL_RenderCopy(render, pictures[ix], NULL, &r);
}

void clearScreen() {
	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	SDL_Rect r = {.x = 0, .y = 0, .w = width, .h = height};
	SDL_RenderFillRect(render, &r);
	SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
	int i;
	for (i = 0; i < height; i += tileSize)
		SDL_RenderDrawLine(render, 0, i, width, i);
	for (i = 0; i < width; i += tileSize)
		SDL_RenderDrawLine(render, i, 0, i, height);
}

static void paint(){
	clearScreen();
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

	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return running;
}
