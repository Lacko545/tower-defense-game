#ifndef MISC_SDL_H_
#define MISC_SDL_H_

#include <SDL2/SDL.h>
#include <string>

using namespace std;

/** loads a surface safely */
SDL_Surface * load_surface(const string & path);


#endif
