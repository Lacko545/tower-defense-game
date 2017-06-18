#ifndef ENGINE_H_
#define ENGINE_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <iterator>
#include <cstring>

#include "game_exceptions.h"
#include "misc_sdl.h"
#include "game_map.h"

/**
 * engine handles initialization of SDL and controls the game loop
 * also contains the game window and its surface
 */
class engine
{
  public:
	/** 
	 * sets up the engine and its constants 
	 * */
	engine(int width, int height, string path);
	/** 
	 * destructor has to delete the main SDL window and surface
	 */
	~engine();
	/** 
	 * starts and controls the game loop
	 * checks the win/lose conditions
	 * 
	 */
	void game_loop();
	/**
	 * initializes SDL
	 */
	bool init();
  private:
	SDL_Window  * main_win;
	SDL_Surface * screen_surface;
	SDL_Surface * win;
	SDL_Surface * lose;
	int screen_width;
	int screen_height;
	game_map map;
	int money;
	int lives;
	int enemy_n;
	Uint32 turn_len;
};

#endif
