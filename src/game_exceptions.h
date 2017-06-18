#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include<exception>

/**
 * Thrown when map file doesn't exist
 */
class MapDoesntExistException : public std::exception
{
  public:
	virtual const char* what() const throw();
};

/**
 * Thrown when map doesn't satisfy format
 */
class MapFormatException : public std::exception
{
  public:

	virtual const char* what() const throw(){
		return "Map format not supported.";
	
	}
};

/**
 * Thrown when SDL fails
 */
class SDLErrorException : public std::exception
{
  public:
	virtual const char* what() const throw();
};

#endif
