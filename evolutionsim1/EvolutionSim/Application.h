#pragma once


#include <odingine/Window.h>
#include <odingine/GLSLProgram.h>
#include <odingine/Camera2D.h>
#include <odingine/InputManager.h>
#include <odingine/SpriteBatch.h>
#include <odingine/GLTexture.h>
#include <odingine/Sprite.h>

#include <Box2D/Box2D.h>

#include "Box.h"
#include "Agent.h"
#include "World.h"
#include "Render.h"

enum class GameState {
	PLAY,
	EXIT
};

/*
struct Game {
	std::map<std::string, std::function<void()>> actions;
	bool should_regenerate = false;
	int step = 2;

	std::string bundle;
	Render renderer;

};
*/


class Application
{
public:
	Application();
	~Application();

	/// Runs the game
	void run();

	/// Initializes the core systems
	void initSystems();

	void initWorld();

	void initRender();

	// initializes level
	void initLevel();

	/// Initializes the shaders
	void initShaders();

	/// Main game loop for the program
	void gameLoop();

	void checkUserInput();

	// update agents
	void updateAgents();

	void updateWorld();

	/// Handles input processing
	void processInput();
	/// Renders the game
	void drawGame();

private:
	/// Member Variables
	const int m_screenWidth = 1024;
	const int m_screenHeight = 500;
	odingine::Window m_window; ///< The game window
	odingine::InputManager m_inputManager; ///< Handles input

	//odingine::SpriteBatch m_agentSpriteBatch; // draws agents

	float m_fps;
	bool m_render = false;
	bool m_fpsToggle = false;

	//int m_currentLevel;

	GameState m_gameState;

	//odingine::SpriteBatch m_spriteBatch;
	//odingine::GLTexture m_texture;	// temp to keep box texture: REMOVE

	//std::vector<Box> m_boxes;
	World* m_world;

	//GeneticAlgorithm* m_GA;

};


