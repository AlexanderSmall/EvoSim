#include "Player.h"
#include "Weapon.h"

#include <SDL/SDL.h>

float AGENT_RADIUS = 60;

Player::Player() : m_currentWeaponIndex(-1)
{
}


Player::~Player()
{
}

void Player::init(float speed, glm::vec2 pos, odingine::InputManager* inputManager, odingine::Camera2D* camera, std::vector<Bullet>* bullets)
{
	m_speed = speed;
	m_position = pos;
	m_health = 300;

	m_inputManager = inputManager;

	m_camera = camera;

	m_bullets = bullets;

	// hard code test colour
	m_color.r = 0;
	m_color.g = 0;
	m_color.b = 128;
	m_color.a = 255;
}

void Player::addWeapon(Weapon* weapon) {
	m_weapons.push_back(weapon);

	// if no weapon equip then equip weapon (-1 = no weapon)
	if (m_currentWeaponIndex == -1) {
		m_currentWeaponIndex = 0;
	}
}

void Player::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies)
{
	if (m_inputManager->isKeyDown(SDLK_w)) {
		m_position.y += m_speed;
	} else if (m_inputManager->isKeyDown(SDLK_s)) {
		m_position.y -= m_speed;
	}

	if (m_inputManager->isKeyDown(SDLK_a)) {
		m_position.x -= m_speed;
	}
	else if (m_inputManager->isKeyDown(SDLK_d)) {
		m_position.x += m_speed;
	}

	// switch between guns
	if (m_inputManager->isKeyDown(SDLK_1) && m_weapons.size() >= 0) {
		m_currentWeaponIndex = 0;
	}
	if (m_inputManager->isKeyDown(SDLK_2) && m_weapons.size() >= 0) {
		m_currentWeaponIndex = 1;
	}

	if (m_currentWeaponIndex != -1) {
		glm::vec2 mouseCoords = m_inputManager->getMouseCoords();
		mouseCoords = m_camera->convertScreenToWorld(mouseCoords);
		
		glm::vec2 playerCenterPos = m_position + glm::vec2(AGENT_RADIUS);
		glm::vec2 direction = glm::normalize(mouseCoords - playerCenterPos);
		
		m_weapons[m_currentWeaponIndex]->update(m_inputManager->isKeyDown(SDL_BUTTON_LEFT),
			playerCenterPos, direction, *m_bullets);
	}

	CollideWithLevel(levelData);
}
