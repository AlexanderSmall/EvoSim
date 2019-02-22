#pragma once

#include <odingine/SpriteBatch.h>

#include <glm/glm.hpp>
#include <GL/GL.h>

const float AGENT_WIDTH = 60;

// forward includes to prevent circular includes
class Zombie;	
class Human;

class Agent
{
public:
	Agent();
	virtual ~Agent();

	virtual void update(const std::vector<std::string>& levelData,
						std::vector<Human*>& humans,
						std::vector<Zombie*>& zombies) = 0;

	bool CollideWithLevel(const std::vector<std::string>& levelData);

	bool collideWithAgent(Agent* agent);

	void draw(odingine::SpriteBatch& spriteBatch);

	// setters

	// return true if dead
	bool setDamage(float damage);

	// getters
	glm::vec2 getPosition() const { return m_position; }
	float getRadius() const { return m_radius;	 }



protected:

	void checkTilePosition(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePosition, float x, float y);

	void collideWithTile(glm::vec2 tilePos);

	glm::vec2 m_position;
	odingine::ColorRGBA8 m_color;

	float m_speed;

	float m_health;

	float m_radius;

	GLuint m_textureID;

	//odingine::GLTexture
};

