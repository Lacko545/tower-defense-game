#include "misc_sdl.h"

using namespace std;


SDL_Texture * load_texture(const string & path, SDL_Renderer * r)
{
	SDL_Texture* new_texture = NULL;
	SDL_Surface* loaded_surface = SDL_LoadBMP( path.c_str() );
	if( loaded_surface == NULL ) { 
		printf( "Unable to load image %s! SDL_image Error: %s\n",
				path.c_str(),
				SDL_GetError() );
	} else { 
		new_texture = SDL_CreateTextureFromSurface( r, loaded_surface );
		if(new_texture == NULL){
			printf( "Unable to create texture from %s! SDL Error: %s\n",
				path.c_str(),
				SDL_GetError() );
		}
		SDL_FreeSurface(loaded_surface);
	}
	return new_texture;
}

SDL_Surface * load_surface(const string & path)
{
	SDL_Surface * loaded_surface=NULL;

	loaded_surface=SDL_LoadBMP(path.c_str());
	if(loaded_surface == NULL){
		printf("Couldn't load surface from %s. SDL_Error:%s",path.c_str(),SDL_GetError());
	}
	return loaded_surface;
}
