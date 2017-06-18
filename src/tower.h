#ifndef TOWER_H_
#define TOWER_H_

#include <string>
#include <stdio.h>
#include <fstream>
#include <algorithm>
#include <vector>

/** 
 * class tower represents towers
 *  towers shoot at nearby enemies at the end of each round
 *  towers have price and cause certain amount of damage
 */


class tower
{
  public:
	/**
	 * tower loads its values(damage, price, range) from a config file
	 */
	tower(const std::string & path);
	/**
	 * image is passed to the tile through a path so it can load the new image
	 */
	std::string get_img_path();
	/** returns the price of the tower */
	int get_price();
	/** returns the tower range */
	int get_range();
  private:
	std::string img_path;
	int damage;
	int price;
	int range;
};

#endif
