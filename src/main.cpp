#include <SDL2/SDL.h>
#include <string>

//#include "misc_sdl.h"
#include "game_map.h"
#include "engine.h"

using namespace std;

int main( int argc, char* args[] )
{
	engine my_engine(816,480,"examples/map1.txt");

	my_engine.game_loop();

	return 0;
}
