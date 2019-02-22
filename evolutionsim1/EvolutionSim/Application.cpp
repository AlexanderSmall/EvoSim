#pragma once
#include "Application.h"
#include "Box.h"
#include "Agent.h"
#include "World.h"

#include <SDL/SDL.h>
#include <GL/GL.h>
#include <Box2D/Box2D.h>

#include <odingine/odingine.h>
#include <odingine/Timing.h>
#include <odingine/ResourceManager.h>

#include <iostream> // REMOVE
#include <random>
#include <ctime>

Application::Application()
{
	// Empty
}

Application::~Application()
{

}

void Application::run() {

	//m_levels.push_back(new Level("Levels/level1.txt"));
	initSystems();
	initWorld(); // generate random world
	gameLoop();

}


void Application::initSystems() {
	// Empty
	odingine::init();
	m_window.create("Evolution Simulator", m_screenWidth, m_screenHeight, 0);	
	// Initialize spritebatch
	//m_spriteBatch.init();
}

void Application::initWorld()
{
	m_world = new World(&m_window);
}

void Application::gameLoop() {
	odingine::FpsLimiter fpsLimiter; // create fps limiter
	fpsLimiter.setMaxFPS(60.0f);

	while (m_gameState == GameState::PLAY) {
		fpsLimiter.begin();

		processInput();



		// SET CAMERA POSITION - use this to track agents. e.g. - the agent doing the best
		//glm::vec2 position(10, 0); // change to moving value
		//m_world->setCameraPosition(position);

		// update Box2D world

		m_world->update();

		//m_world->compute();

		// draw world
		m_world->render();

		fpsLimiter.end();


		//maybe swap buffers?
		//m_window.swapBuffer();
	}

}

void Application::updateAgents()
{
	
}


void Application::processInput() {
	SDL_Event evnt;

	int clickX = 0;
	int clickY = 0;

	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			//Exit the game here!
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			// place agent on click
			//clickX = _inputManager.getMouseCoords().x;
			//clickY = _inputManager.getMouseCoords().y;
			m_world->spawnAgent(clickX, clickY);
			std::cout << "x: " << clickX << "y: " << clickY << std::endl;
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;
		}

	}

}

