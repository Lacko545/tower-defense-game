#include "enemy.h"

using namespace std;

SDL_Surface * enemy::get_surface()
{
	return img;
}

bool enemy::take_damage(int dmg){
	hp-=dmg;
	return( hp<=0 ? true : false );
}

int enemy::percent_hp()
{
	return hp/100;
}
