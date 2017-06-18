#ifndef ENEMY_H_
#define ENEMY_H_

#include <vector>
#include <utility>
#include <SDL2/SDL.h>

#include "misc_sdl.h"

/**
 * enemy class represents enemy entities.
 * enemies have a different surface for when they are damaged
 */

class enemy
{
  public:
	  /**
	   * loads enemy images and sets up enemy properties
	   */
	enemy():hp(3),value(1)
	{
		img=SDL_LoadBMP("src/assets/enemy.bmp");
		dmg_img=load_surface("src/assets/enemy_dmg.bmp");
	}
	/**
	 * destroys surfaces
	 */
	~enemy()
	{
		if(img!=NULL) SDL_FreeSurface(img);
		if(dmg_img!=NULL) SDL_FreeSurface(dmg_img);
	}

	/**
	 * returns enemy image to be drawn on the tile instead of empty tile
	 */
	SDL_Surface * get_surface();
	/**
	 * applies damage to enemy, returns the reward if enemy is killed
	 */
	bool take_damage(int dmg);
	/**
	 * returns HP in percentage;
	 */
	int percent_hp();
  private:

	/** default image */
	SDL_Surface * img;
	/** damage image */
	SDL_Surface * dmg_img;
	/** enemy hp */
	int hp;
	/** gold value of enemy */
	int value;
};

#endif
