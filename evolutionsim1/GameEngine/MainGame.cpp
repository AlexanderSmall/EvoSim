#include "MainGame.h"

#include <odingine/odingine.h>
#include <odingine/Errors.h>
#include <odingine/ResourceManager.h>
#include <odingine/GLTexture.h>

#include <iostream>
#include <string>

MainGame::MainGame() : 
	_screenWidth(1024),
	_screenHeight(512),
	_gameState(GameState::PLAY),
	_time(0.0f),
	_maxFPS(60.0f)
{
	_camera.init(_screenWidth, _screenHeight);
}

MainGame::~MainGame()
{

}

void MainGame::run()
{
	initSystems();	

	//_sprites.push_back(new odingine::Sprite());
	//_sprites.back()->init(0.0f, 0.0f, _screenWidth / 4, _screenWidth / 4, "Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

	//_sprites.push_back(new odingine::Sprite());
	//_sprites.back()->init(_screenWidth / 4, 0.0f, _screenWidth / 4, _screenWidth / 4, "Textures/jimmyJump_pack/PNG/CharacterRight_Jump.png");

	//_playerTexture = ImageLoader::loadPNG("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

	gameLoop();
}

void MainGame::initSystems()
{
	odingine::init();

	_window.create("Name of Game", _screenWidth, _screenHeight, 0);

	initShaders();

	_spriteBatch.init();

	_fpsLimiter.init(_maxFPS);
}
void MainGame::initShaders()
{
	_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();

}


void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		_fpsLimiter.begin();

		processInput();
		_time += 0.01;

		_camera.update();

		for (int i = 0; i < _bullets.size();) {
			if (_bullets[i].update() == true) {
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
			}
			else {
				i++;
			}
		}

		drawGame();

		_fps = _fpsLimiter.end();

		// print every 10 frames
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 10) {
			//std::cout << _fps << std::endl;
			frameCounter = 0;
		}

	}

}

void MainGame::processInput()
{
	SDL_Event evnt;
	
	const float SCALE_SPEED = 0.1f;
	
	while (SDL_PollEvent(&evnt) == true) {
		switch (evnt.type) {
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;

		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;

		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		}
	}

	if (_inputManager.isKeyPressed(SDLK_w)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -1.0f));
	}
	if (_inputManager.isKeyPressed(SDLK_a)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(1.0f, 0.0f));
	}
	if (_inputManager.isKeyPressed(SDLK_s)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, 1.0f));
	}
	if (_inputManager.isKeyPressed(SDLK_d)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(-1.0f, 0.0f));
	}
	if (_inputManager.isKeyPressed(SDLK_q)) {
		_camera.setScale(_camera.getScale() + SCALE_SPEED);
	}
	if (_inputManager.isKeyPressed(SDLK_e)) {
		_camera.setScale(_camera.getScale() - SCALE_SPEED);
	}

	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
		glm::vec2 mouseCoords = _inputManager.getMouseCoords();
		mouseCoords = _camera.convertScreenToWorld(mouseCoords);
		//std::cout << mouseCoords.x << " , " << mouseCoords.y << std::endl;

		glm::vec2 playerPosition(0.0f);
		glm::vec2 direction = mouseCoords - playerPosition;
		direction = glm::normalize(direction);


		_bullets.emplace_back(playerPosition, direction, 1.00f, 1000);

	}
}

void MainGame::drawGame()
{

	// set the base deth to 1.0
	glClearDepth(1.0);
	// clear colour and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_colorProgram.use();

	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, _playerTexture.id);
	
	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	//GLuint timeLocation = _colorProgram.getUniformLocation("time");
	//glUniform1f(timeLocation, _time);

	// set orthographic camera matrix
	GLuint pLocation = _colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();

	// set the camera matrix
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	_spriteBatch.begin();

	//draw batches
	glm::vec4 pos(0.0f, 0.0f, 50.0f, 50.0f);  // test triangle
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static odingine::GLTexture texture = odingine::ResourceManager::getTexture("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
	odingine::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;
	_spriteBatch.draw(pos, uv, texture.id, 0.0f, color);

	for (int i = 0; i < _bullets.size(); i++) {
		_bullets[i].draw(_spriteBatch);
	}

	_spriteBatch.end();
	_spriteBatch.renderBatch();

	// unbind textures
	glBindTexture(GL_TEXTURE_2D, 0);

	// disbale the shader
	_colorProgram.unuse();

	// swap out buffer and draw everything to the screen
	_window.swapBuffer();
}

