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
	m_window.create("EvoSim", m_screenWidth, m_screenHeight, 0);	
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

	double beforeRender = 0;
	double timeToRender = 0;
	// Implement Fixed Time Step
	while (m_gameState == GameState::PLAY) {

		fpsLimiter.begin();		// begin fps counter
		double seconds = difftime(time(0), startEpochTime);

		//beforeRender = time(0);
		//timeToRender = difftime(time(0), beforeRender);
		//seconds -= timeToRender;

		if (seconds < 100.0f) {	



			m_world->update();      // update Box2D world


			m_world->render(); 		// draw world


			processInput();			// process the users input
			checkUserInput();

			m_fps = fpsLimiter.end();		// end fps counter
			if (m_fpsToggle) std::cout << "fps: " << m_fps << std::endl;


		} else {
			// reset world

			// Evole Agents
			m_world->EvolveAgents();

			// increment epoch
			m_world->incrementEpoch();

			startEpochTime = time(0);	// reset timer
		}
	}

}

void Application::checkUserInput()
{
	if (m_inputManager.isKeyDown(SDLK_r)) {	// Render bodies to the screen
		if (m_renderToggle) {
			m_world->setFlags(false);
			m_renderToggle = false;
		}
		else {
			m_world->setFlags(true);
			m_renderToggle = true;
		}
	}

	if (m_inputManager.isKeyDown(SDLK_f)) {	// Display fps counter
		if (m_fpsToggle) {	// turn off fps
			m_fpsToggle = false;
		}
		else {	// turn on fps
			m_fpsToggle = true;
		}
	}

	if (m_inputManager.isKeyDown(SDLK_p)) {	// Follow the best agent with the camera
		if (m_followBestAgentToggle) {
			m_world->FollowBestAgent();
			m_followBestAgentToggle = false;
		}
		else {	// turn on fps
			m_followBestAgentToggle = true;
		}
	}

	if (m_inputManager.isKeyDown(SDLK_t)) {	// clear all agents from the world
		m_world->clearAgents();
	}

	if (m_inputManager.isKeyDown(SDLK_g)) {	// add one agent at 10, 10
		m_world->spawnAgent(-1, 10, 10);
	}

	if (m_inputManager.isKeyDown(SDLK_z)) {	// Zoom in
		m_world->zoomIn();
	}

	if (m_inputManager.isKeyDown(SDLK_x)) {	// Zoom out
		m_world->zoomOut();
	}

	if (m_inputManager.isKeyDown(SDLK_k)) {	// Zoom in
		m_world->moveLeft();
	}

	if (m_inputManager.isKeyDown(SDLK_l)) {	// Zoom out
		m_world->moveRight();
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

