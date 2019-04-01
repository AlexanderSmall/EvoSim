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

	//m_world->createAgents();
}

void Application::gameLoop() {
	odingine::FpsLimiter fpsLimiter; // create fps limiter
	fpsLimiter.setMaxFPS(10.0f);

	time_t startEpochTime = time(0);

	double beforeRender = 0;
	double timeToRender = 0;
	// Implement Fixed Time Step
	while (m_gameState == GameState::PLAY) {

		fpsLimiter.begin();		// begin fps counter
		double seconds = difftime(time(0), startEpochTime);

		//beforeRender = time(0);
		checkUserInput();
		//timeToRender = difftime(time(0), beforeRender);
		//seconds -= timeToRender;

		if (seconds < 100.0f) {	


			processInput();			// process the users input
			m_world->update();      // update Box2D world
			
			
			// TEST: REMOVE REENDERING TIME FROM TIMER - may not work
			beforeRender = time(0);
			m_world->render(); 		// draw world
			timeToRender = difftime(time(0), beforeRender);
			seconds -= timeToRender;

		} else {
			// reset world
			std::cout << "=========" << std::endl;

			// increment age of ages
			//m_world->incrementAgentAge();

			//m_world->clearAgents(); // uncomment

			// Evole Agents
			m_world->EvolveAgents();

			//m_world->clearAgents(); // clear the agents from the world 

			// Create new and existing Agents
			//m_world->createAgents();	// temp function - call create NEW agents

			//m_world->OutputAgentID();

			m_world->incrementEpoch();

			startEpochTime = time(0);	// reset timer

		}

		m_fps = fpsLimiter.end();		// end fps counter
		if (m_fpsToggle) std::cout << "fps: " << m_fps << std::endl;
	}

}

void Application::checkUserInput()
{
	if (m_inputManager.isKeyDown(SDLK_r)) {	// Render bodies to the screen
		if (m_render) {
			m_world->setFlags(false);
			m_render = false;
		}
		else {
			m_world->setFlags(true);
			m_render = true;
		}
	}

	if (m_inputManager.isKeyDown(SDLK_f)) {	// Render bodies to the screen
		if (m_fpsToggle) {	// turn off fps
			m_fpsToggle = false;
		}
		else {	// turn on fps
			m_fpsToggle = true;
		}
	}

	if (m_inputManager.isKeyDown(SDLK_t)) {	// Render bodies to the screen
		m_world->clearAgents();
	}

	if (m_inputManager.isKeyDown(SDLK_g)) {	// Render bodies to the screen
		m_world->spawnAgent(-1, 10, 10);
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
			m_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			m_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			m_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_inputManager.pressKey(evnt.button.button);
			// place agent on click
			//m_world->spawnAgent(clickX, clickY);
			break;
		case SDL_MOUSEBUTTONUP:
			m_inputManager.releaseKey(evnt.button.button);
			break;
		}

	}

}

