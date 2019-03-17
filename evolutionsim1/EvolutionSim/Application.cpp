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
	fpsLimiter.setMaxFPS(60.0f);

	time_t startEpochTime = time(0);

	while (m_gameState == GameState::PLAY) {

		double seconds = difftime(time(0), startEpochTime);	
		if (seconds < 60) {	

			//fpsLimiter.begin();		// begin fps counter

			processInput();			// process the users input
			m_world->update();      // update Box2D world
			m_world->render(); 		// draw world

			// fpsLimiter.end();		// end fps counter
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

			m_world->incrementEpoch();

			startEpochTime = time(0);	// reset timer

		}



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
			//m_world->spawnAgent(clickX, clickY);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;
		}

	}

}

