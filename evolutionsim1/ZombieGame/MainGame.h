#pragma once

#include <odingine/Window.h>
#include <odingine/GLSLProgram.h>
#include <odingine/Camera2D.h>
#include <odingine/InputManager.h>
#include <odingine/SpriteBatch.h>

#include "Player.h"
#include "Level.h"
#include "Bullet.h"

class Zombie;

enum class GameState {
	PLAY,
	EXIT
};

class MainGame
{
public:
    MainGame();
    ~MainGame();

    /// Runs the game
    void run();

private:
    /// Initializes the core systems
    void initSystems();

	// initializes level
	void initLevel();

    /// Initializes the shaders
    void initShaders();

    /// Main game loop for the program
    void gameLoop();

	// update agents
	void updateAgents();

	//update bullets
	void updateBullets();

    /// Handles input processing
    void processInput();
    /// Renders the game
    void drawGame();

private:
    /// Member Variables
	odingine::Window _window; ///< The game window
	odingine::GLSLProgram _textureProgram; ///< The shader program
	odingine::InputManager _inputManager; ///< Handles input
	odingine::Camera2D _camera; ///< Main Camera

	odingine::SpriteBatch m_agentSpriteBatch; // draws agents

	std::vector<Level*> m_levels;

	int m_screenWidth;
	int m_screenHeight;

	float m_fps;

	int m_currentLevel;

	std::vector<Human*> m_humans; // vector of all humans
	std::vector<Zombie*> m_zombies; // vector of all humans
	Player* m_player;

	std::vector<Bullet> m_bullets;

	GameState m_gameState;
};

