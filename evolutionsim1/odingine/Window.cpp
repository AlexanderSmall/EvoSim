#include "Window.h"

#include "Errors.h"

#include <iostream>

namespace odingine {

	Window::Window()
	{
	}


	Window::~Window()
	{
	}

	int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags)
	{

		// set up window flags
		Uint32 flags = SDL_WINDOW_OPENGL;

		if (currentFlags & INVISIBLE) {
			flags |= SDL_WINDOW_HIDDEN;
		}

		if (currentFlags & FULLSCREEN) {
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if (currentFlags & BORDERLESS) {
			flags |= SDL_WINDOW_BORDERLESS;
		}


		// Initialise SDL with video and events.
		atexit(SDL_Quit); // SDL_Quit is safe to call even if SDL_Init failed.
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			std::cerr << "SDL_Init failed!" << std::endl;
			throw std::runtime_error{ SDL_GetError() };
		}

		// open SDL window
		_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

		if (_sdlWindow == nullptr) {
			fatalError("SDL Window could not be created");
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		// set up OPenGL context
		SDL_GLContext glContext = SDL_GL_CreateContext(_sdlWindow);
		//gl_context_ptr glContext{ SDL_GL_CreateContext(_sdlWindow) };
		if (glContext == nullptr) {
			fatalError("SDL_GL context could not be created");
		}

		// Initialise GLEW.
		//glewExperimental = GL_TRUE;

		// set up glew
		GLenum error = glewInit();
		if (error != GLEW_OK) {
			fatalError("Could not initialise glew");
		}

		// check OpenGL version
		std::cout << "*** OpenGL Version: " << glGetString(GL_VERSION) << " *** \n";

		// set background color
		//glClearColor(1.0f, 0.5f, 0.5f, 1.0f);

		// Try to set VSync; failure is OK here.
		if (SDL_GL_SetSwapInterval(1) < 0)
		{
			std::cout << "[Warning] Failed to set VSync: " << SDL_GetError()
				<< std::endl;
		}

		// enable alpha blending
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		return 0;
	}

	void Window::swapBuffer()
	{

		// Swap out buffer and draw 
		SDL_GL_SwapWindow(_sdlWindow);
	}

}


