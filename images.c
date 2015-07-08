#include <SDL2/SDL.h>
#include <Imlib2.h>

#include "gui.h"

void loadPics(){
	int numPics = 0;
	char picturename[80];
	FILE *fp = fopen("mdls/contents.txt", "r");
	while(fgets(picturename, 80, fp)){
		pictures = realloc(pictures, sizeof(SDL_Texture*)*(1+numPics));
		picturename[strlen(picturename)-1] = '\0';
		Imlib_Image img = imlib_load_image(picturename);
		imlib_context_set_image(img);
		SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(imlib_image_get_data(), imlib_image_get_width(), imlib_image_get_height(), 32, imlib_image_get_width()*4, 0xFF0000, 0x00FF00, 0x0000FF, 0xFF000000);
		SDL_SetTextureBlendMode(pictures[numPics] = SDL_CreateTextureFromSurface(render, surface), SDL_BLENDMODE_BLEND);
		SDL_FreeSurface(surface);
		imlib_free_image();
		numPics++;
	}
	fclose(fp);
}
