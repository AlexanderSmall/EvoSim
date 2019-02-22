#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <odingine/SpriteBatch.h>

#include <string>

const int BULLET_RADIUS = 5;

// forward declares
class Human;
class Zombie;
class Agent;

class Bullet
{
public:
	Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed);
	~Bullet(); 

	bool update(const std::vector<std::string>& levelData);

	void draw(odingine::SpriteBatch& spriteBatch);

	bool collisionWithAgent(Agent* agent);

	//getters
	float getDamage() const { return m_damage;	 }

private:
	bool collideWithMap(const std::vector<std::string>& levelData);
	
	float m_damage;
	glm::vec2 m_position;
	glm::vec2 m_direction;
	float m_speed;

};

