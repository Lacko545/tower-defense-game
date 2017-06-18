#ifndef GAME_MAP_H_
#define GAME_MAP_H_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <exception>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <queue>
#include <map>
#include <set>

#include "game_exceptions.h"
#include "tower.h"
#include "enemy.h"
#include "misc_sdl.h"

/**
 * a virtual class which represents a generic tile
 * there are 3 types of tyles
 * empty tiles - these can contain enemies. it can be an end tile or a beginning tile.
 * wall tile - these cannot be passed through
 * tower tile - towers can be built here
 */ 

class tile
{
  public:
	tile(int x,int y): x_pos(x),y_pos(y){}
	virtual ~tile();
	virtual void move(tile & dest){}
	virtual int remove(){return 0;}
	virtual void occupy(enemy * in_occupant){}
	virtual void attack(){};
	virtual bool occupied(){ return true; }
	virtual int get_range(){return 0;}
	virtual bool take_damage(int amount){ return false; };
	virtual bool is_end(){return false;}
	virtual bool is_start(){return false;}
	virtual void spawn(){}
	virtual int build(int money){
		printf("this should never happen\n");
		return 0;
	}
	virtual SDL_Surface * get_surface(){
		return this->img;
	}
  protected:
	int x_pos;
	int y_pos;
	SDL_Surface * img;

};

/**
 * a wall tile, enemies can't move through this tile
 */ 
class wall_tile: public tile
{
  public:
	wall_tile(int x,int y);
};

/**
 * a spot for a tower
 * empty by default
 * a tower can be built on it by clicking on it
 * tower object is contained through a pointer
 */ 
class tower_tile: public tile
{
  public:
	/*
	 * sets the constants from the tower config file
	 */
	tower_tile(int x, int y);
	~tower_tile();
	virtual int get_range();
	/*
	 * builds a tower, provided that the amount of money is greater or equal
	 * to the cost of the tower
	 */
	virtual int build(int money);
  private:
	tower * building;
};


/**
 * an empty tile that enemies can move through
 * can contain an enemy object in a pointer
 */ 
class empty_tile: public tile
{
  public:
	/**
	 * initializes empty tile
	 */
	empty_tile(int x, int y);
	/**
	 * initializes tile based on a character from map file
	 * # = wall
	 * ' '=free space
	 * E = finish line
	 * S = starting point
	 * _ = spot for a tower
	 * 
	 */
	empty_tile(int x, int y, char c);
	/**
	 * spawns an enemy on the tile
	 */
	virtual void spawn();
	/**
	 * checks whether a tile is occupied by a tower/tile/enemy
	 */
	virtual bool occupied();
	/**
	 * checks whether tile is the finish point for the enemies
	 */
	virtual bool is_end();
	/**
	 * checks whether the tile is the spawn point for enemies
	 */
	virtual bool is_start();
	/**
	 * returns tile surface for blitting
	 */
	virtual SDL_Surface * get_surface();
	/**
	 * removes an enemy from the tile
	 */
	virtual int remove();
	/**
	 * moves the nemy to a different tile
	 */
	virtual void move(tile & dest);
	/**
	 * inserts an enemy into the tile
	 */
	virtual void occupy(enemy * in_occupant);
	/**
	 * takes damage, returns true if it kills the enemy
	 */
	virtual bool take_damage(int amount);

  private:
	enemy * occupant;
	bool start;
	bool end;
};


class game_map
{
  public:
	/*
	 * Constructs tile map of tile objects based on a text file
	 * map is represented as a 2D array of pointers
	 * throws exception if the map does not exist, contains characters
	 * that don't belong on the map or if all the lines aren't the same length
	 */
	game_map(const std::string & path);
	~game_map();
	/**
	 * Makes all the towers shoot enemies within their range
	 * returns amount of destroyed enemies
	 */
	int shoot();
	/**
	 * moves all enemies on the map
	 * if spawn is true, also spawns an enemy on the spawn
	 * if an enemy passes through the exit, deletes it and returns 1
	 */
	int move(bool spawn);
	/**
	 * iterates through the 2D array of tiles
	 * gets surface of each one and blits it on the screen
	 */
	void draw_map(SDL_Surface *& screen);
	/**
	 * finds shortest path through the map
	 * returns a vector of indexes of all the tiles
	 * on the path, in order from beginning to the end
	 */
	std::vector<std::pair<int,int>> find_path() const;
	/**
	 * handles a click on the screen. If it's within the game area,
	 * applies build() to the tile, which builds the tower
	 * provided there is not a tower there already
	 */
	int click(int x, int y, int money);
	/**
	 * returns the amount of enemies on the map
	 */
	int enemy_cnt();
  private:
	int x_dim;
	int y_dim;
	int tile_width;
	std::vector<std::pair<int,int>> shortest_path;
	int cols;
	int rows;
	std::vector< std::string > map_data; 
	tile *** tile_map;
	int enemy_n;
};

#endif
