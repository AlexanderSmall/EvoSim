#include "Errors.h"
#include <cstdlib>

#include <SDL/SDL.h>
#include <Gl/glew.h>

#include <iostream>

namespace odingine {

	// Create better erro checking later 
	void fatalError(std::string errorString) {
		std::cout << errorString << std::endl;

		GLenum err;
		err = glGetError();
		std::cout << "Error code: " << err << std::endl;

		std::cout << "Enter any key to quit...";
		int tmp;
		std::cin >> tmp;
		SDL_Quit();
		exit(-1);
	}

}