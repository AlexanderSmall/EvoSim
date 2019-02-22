#include "MainGame.h"
#include "Zombie.h"
#include "Weapon.h"

#include <SDL/SDL.h>
#include <GL/GL.h>

#include <odingine/odingine.h>
#include <odingine/Timing.h>

#include <iostream> // REMOVE
#include <random>
#include <ctime>

const float Z_SPEED = 1.0f;	// zombie speed

MainGame::MainGame() : 
	m_screenWidth(1024),
	m_screenHeight(768),
	m_gameState(GameState::PLAY),
	m_fps(0.0f),
	m_player(nullptr)
{
    // Empty
}

MainGame::~MainGame() 
{
	for (int i = 0; i < m_levels.size(); i++) {
		delete m_levels[i];
	}
}

void MainGame::run() {

	//m_levels.push_back(new Level("Levels/level1.txt"));
	initSystems();

	initLevel();

	gameLoop();

}

void MainGame::initSystems() {
	// Empty
	odingine::init();

	_window.create("Zombie Game", m_screenWidth, m_screenHeight, 0);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

	initShaders();
	m_agentSpriteBatch.init();

	_camera.init(m_screenWidth, m_screenHeight);
}

void MainGame::initLevel()
{
	// level 1
	m_levels.push_back(new Level("Levels/level1.txt"));
	m_currentLevel = 0;

	m_player = new Player();
	m_player->init(5.0f, m_levels[m_currentLevel]->getStartPlayerPos(), &_inputManager, &_camera, &m_bullets);

	m_humans.push_back(m_player);

	// randm generators not generating random numbers - all humans in the same place

	// set up random engien to generate humans between map dimentions
	static std::mt19937 randEng;
	randEng.seed(time(nullptr));
	static std::uniform_int_distribution<int> randXPos(2, m_levels[m_currentLevel]->getWidth() - 2);
	static std::uniform_int_distribution<int> randYPos(2, m_levels[m_currentLevel]->getHeight() - 2);

	const float H_SPEED = 1.0f;	// human speed
	// iniate and add all humans
	for (int i = 0; i < m_levels[m_currentLevel]->getNumOfHumans(); i++) {
		m_humans.push_back(new Human);
		glm::vec2 humanPos(randXPos(randEng) * TILE_WIDTH, randYPos(randEng) * TILE_WIDTH);	// human position
		m_humans.back()->init(H_SPEED, humanPos);
	}

	// add zombies
	const std::vector<glm::vec2>& zombiePositions = m_levels[m_currentLevel]->getStartZombiePos();
	for (int i = 0; i < zombiePositions.size(); i++) {
		m_zombies.push_back(new Zombie);
		glm::vec2 zombiePos(randXPos(randEng) * TILE_WIDTH, randYPos(randEng) * TILE_WIDTH);	// human position
		m_zombies.back()->init(Z_SPEED, zombiePositions[i]);
	}

	//set up players weapon
	m_player->addWeapon(new Weapon("pistol", 30, 1, 0.5f, 10.0f, 10.0f));
	m_player->addWeapon(new Weapon("shotgun", 60, 20, 1.0f, 10.0f, 4.0f));


}

void MainGame::initShaders() {
    // Compile our color shader
	_textureProgram.compileShaders("Shaders/vertexShader.vs", "Shaders/fragmentShader.fs");
	_textureProgram.addAttribute("vertexPosition");
	_textureProgram.addAttribute("vertexColor");
	_textureProgram.addAttribute("vertexUV");
	_textureProgram.linkShaders();
}

void MainGame::gameLoop() {
	odingine::FpsLimiter fpsLimiter; // create fps limiter
	fpsLimiter.setMaxFPS(60);


	while (m_gameState == GameState::PLAY) {
		fpsLimiter.begin();

		processInput();

		updateAgents();		// update all the agents

		updateBullets();	// update bullets

		_camera.setPosition(m_player->getPosition());

		_camera.update();

		drawGame();

		fpsLimiter.end();
	}

}

void MainGame::updateAgents()
{
	// update all humans
	for (int i = 0; i < m_humans.size(); i++) {
		m_humans[i]->update(m_levels[m_currentLevel]->getLevelData(),
							m_humans,
							m_zombies);

	}

	// update all zombies
	for (int i = 0; i < m_zombies.size(); i++) {
		m_zombies[i]->update(m_levels[m_currentLevel]->getLevelData(),
			m_humans,
			m_zombies);
	}

	//update zombie collisions
	
	for (int i = 0; i < m_zombies.size(); i++) {
		// zombie - zombie collision
		for (int j = i + 1; j < m_zombies.size(); j++) {
			m_zombies[i]->collideWithAgent(m_zombies[j]);
		}
		// zombie - human collision
		for (int j = 1; j < m_humans.size(); j++) {
			if (m_zombies[i]->collideWithAgent(m_humans[j])) {
				// add new zombie
				m_zombies.push_back(new Zombie);
				m_zombies.back()->init(Z_SPEED, m_humans[j]->getPosition());
				// delete human
				delete m_humans[j];
				m_humans[j] = m_humans.back();
				m_humans.pop_back();
			}
		}
		// player collision
		if (m_zombies[i]->collideWithAgent(m_player)) {
			std::cout << "YOU LOSE" << std::endl;
		}
	
	}

	// update human collisions

	for (int i = 0; i < m_humans.size(); i++) {
		for (int j = i + 1; j < m_humans.size(); j++) {
			m_humans[i]->collideWithAgent(m_humans[j]);
		}
	}

}

void MainGame::updateBullets()
{
	// update and collide with map
	for (int i = 0; i < m_bullets.size();) {
		if (m_bullets[i].update(m_levels[m_currentLevel]->getLevelData())) {	// if true bullet collisoin then delete
			m_bullets[i] = m_bullets.back();
			m_bullets.pop_back();
		}
		else {
			i++;
		}
	}

	bool wasBulletRemoved;

	// update and collide with agents (humans/zombies)
	for (int i = 0; i < m_bullets.size(); i++) {
		wasBulletRemoved = false;
		for (int j = 0; j < m_zombies.size();) {
			if (m_bullets[i].collisionWithAgent(m_zombies[j])) {	// check collision
				// Damage zombie and kill if health < 0
				if (m_zombies[j]->setDamage(m_bullets[i].getDamage())) { 	// if zombie dies - remove 
					delete m_zombies[j];
					m_zombies[j] = m_zombies.back();
					m_zombies.pop_back();
				}
				else {
					j++;
				}

				// Remove bullet
				m_bullets[i] = m_bullets.back();
				m_bullets.pop_back();
				wasBulletRemoved = true;
				i--; 	// make sure not to skip bullet
				break;
			}
			else {
				j++;
			}
		}

		if (wasBulletRemoved == false) {
			for (int j = 1; j < m_humans.size();) {	// j = 1 to avoid collision with player
				if (m_bullets[i].collisionWithAgent(m_humans[j])) {	// check collision
					// Damage zombie and kill if health < 0
					if (m_humans[j]->setDamage(m_bullets[i].getDamage())) { 	// if zombie dies - remove 
						delete m_humans[j];
						m_humans[j] = m_humans.back();
						m_humans.pop_back();
					}
					else {
						j++;
					}

					// Remove bullet
					m_bullets[i] = m_bullets.back();
					m_bullets.pop_back();
					wasBulletRemoved = true;
					i--; 	// make sure not to skip bullet
					break;
				}
				else {
					j++;
				}
			}
		}
	}

}

void MainGame::processInput() {
    SDL_Event evnt;

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
                break;
            case SDL_MOUSEBUTTONUP:
				_inputManager.releaseKey(evnt.button.button);
                break;
        }
    
	}

}

void MainGame::drawGame() {
    // Set the base depth to 1.0
    glClearDepth(1.0);
    // Clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// bind texture program
	_textureProgram.use();

	//Draw code goes here
	glActiveTexture(GL_TEXTURE0);

	// make sure shader uses texture 0
	GLint textureUniform = _textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	// get camera matrix
	glm::mat4 projectionMatrix = _camera.getCameraMatrix();
	// upload camera to shader
	GLint pUniform = _textureProgram.getUniformLocation("transformationMatrix");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	//Draw level
	m_levels[m_currentLevel]->draw();


	m_agentSpriteBatch.begin(); // start spriteBatch
	//Draw all agents
	for (int i = 0; i < m_humans.size(); i++) {
		m_humans[i]->draw(m_agentSpriteBatch);
	}

	//draw zombies
	for (int i = 0; i < m_zombies.size(); i++) {
		m_zombies[i]->draw(m_agentSpriteBatch);
	}

	// draw bullets
	for (int i = 0; i < m_bullets.size(); i++) {
		m_bullets[i].draw(m_agentSpriteBatch);
	}

	m_agentSpriteBatch.end();	// end spriteBatch
	m_agentSpriteBatch.renderBatch();	// render the agents in the spriteBatch



	//unbind texture program
	_textureProgram.unuse();

	//Swap our buffer and draw everything to the screen!
	_window.swapBuffer();
}