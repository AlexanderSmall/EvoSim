#include "Bullet.h"
#include "Agent.h"
#include "Human.h"
#include "Zombie.h"
#include <odingine/ResourceManager.h>
#include "Level.h"

Bullet::Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed) : 
	m_position(position),
	m_direction(direction),
	m_damage(damage),
	m_speed(speed)
{
}

Bullet::~Bullet()
{
}

bool Bullet::update(const std::vector<std::string>& levelData)
{
	m_position += m_direction * m_speed;
	return collideWithMap(levelData);
}

void Bullet::draw(odingine::SpriteBatch & spriteBatch)
{
	glm::vec4 destRect(m_position.x + BULLET_RADIUS, m_position.y + BULLET_RADIUS, BULLET_RADIUS * 2, BULLET_RADIUS * 2);
	
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	
	odingine::ColorRGBA8 color;
	color.r = 120;
	color.g = 200;
	color.b = 30;
	color.a = 255;

	spriteBatch.draw(destRect, uvRect, odingine::ResourceManager::getTexture("Textures/circle.png").id, 0.0f, color);
}

bool Bullet::collisionWithAgent(Agent * agent)
{
	float AGENT_RADIUS = 60.0f;

	const float MIN_DISTANCE = AGENT_RADIUS + BULLET_RADIUS;

	glm::vec2 PointA = m_position;
	glm::vec2 PointB = agent->getPosition() + glm::vec2(agent->getRadius());

	glm::vec2 disVector = PointA - PointB;

	float distance = glm::length(disVector);

	float collisionDistance = MIN_DISTANCE - distance;
	if (collisionDistance > 0) {
		return true;
	}
	return false;
}

bool Bullet::collideWithMap(const std::vector<std::string>& levelData)
{
	glm::ivec2 gridPos;
	gridPos.x = floor(m_position.x / (float)TILE_WIDTH);
	gridPos.y = floor(m_position.y / (float)TILE_WIDTH);

	// if outside world return 
	if (gridPos.x < 0 || gridPos.x >= levelData[0].size() ||
		gridPos.y < 0 || gridPos.y >= levelData.size()) {
		return true;
	}
	
	return (levelData[gridPos.y][gridPos.x] != '.');
}
