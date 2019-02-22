
#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>

#include "odingine.h"

namespace odingine {

	int init() {
		// Initialise SDL
		SDL_Init(SDL_INIT_EVERYTHING);

		// TEll SDL that we want to double buffer window so we don't get any flickering
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);	// two videos used to prevent flickering
	
		return 0;
	}

}