#pragma once

#include "Human.h"
#include "Bullet.h"

#include <odingine/InputManager.h>
#include <odingine/Camera2D.h>

class Weapon;
class Bullet;

class Player : public Human
{
public:
	Player();
	~Player();

	void init(float speed, glm::vec2 pos, odingine::InputManager* inputManager, odingine::Camera2D* camera, std::vector<Bullet>* bullets);

	void addWeapon(Weapon* weapon);

	void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies) override;
 

private:
	odingine::InputManager* m_inputManager;

	std::vector<Weapon*> m_weapons;
	std::vector<Bullet>* m_bullets;
	int m_currentWeaponIndex;

	odingine::Camera2D* m_camera;
};

