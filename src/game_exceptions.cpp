#include "game_exceptions.h"

using namespace std;



const char* MapDoesntExistException::what() const throw()
{
	return "Map file doesn't exist.";
}

const char* SDLErrorException::what() const throw()
{
	return "Couldn't load SDL.";
}
