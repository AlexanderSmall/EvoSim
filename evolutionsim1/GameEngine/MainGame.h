#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <odingine/GLSLProgram.h>
#include <odingine/Sprite.h>
#include <odingine/GLTexture.h>
#include <odingine/Window.h>
#include <odingine/Camera2D.h>
#include <odingine/SpriteBatch.h>
#include <odingine/InputManager.h>
#include <odingine/Timing.h>

#include "Bullet.h"

#include <vector>

	enum class GameState { PLAY, EXIT };

	class MainGame
	{
	public:
		MainGame();
		~MainGame();

		void run();

	private:
		void initSystems();
		void initShaders();
		void gameLoop();
		void processInput();
		void drawGame();

		odingine::Window _window;
		int _screenWidth;
		int _screenHeight;
		GameState _gameState;

		//std::vector <odingine::Sprite*> _sprites;
		//Sprite _sprite;	// test sprite

		odingine::GLSLProgram _colorProgram;
		
		odingine::Camera2D _camera;

		odingine::SpriteBatch _spriteBatch;

		odingine::InputManager _inputManager;

		odingine::FpsLimiter _fpsLimiter;

		std::vector<Bullet> _bullets;

		float _maxFPS;
		float _fps;

		float _time;

	};
