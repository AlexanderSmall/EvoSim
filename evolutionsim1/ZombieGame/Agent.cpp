#include "Agent.h"
#include "level.h"

#include <odingine/ResourceManager.h>

#include <algorithm>

Agent::Agent() : m_radius((float)AGENT_WIDTH / 2.0f) // may go wrong
{
}


Agent::~Agent()
{
}

void Agent::draw(odingine::SpriteBatch & spriteBatch)
{
	static int textureID = odingine::ResourceManager::getTexture("Textures/circle.png").id;

	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 destRect;
	destRect.x = m_position.x;
	destRect.y = m_position.y;
	destRect.z = AGENT_WIDTH;
	destRect.w = AGENT_WIDTH;

	// draw agent
	spriteBatch.draw(destRect, uvRect, textureID, 0.0f, m_color);
}

bool Agent::setDamage(float damage)
{
	m_health -= damage;
	if (m_health <= 0) {
		return true;
	}
	return false;
}

void Agent::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies)
{

}

bool Agent::CollideWithLevel(const std::vector<std::string>& levelData)
{
	std::vector<glm::vec2> collideTilePosition;
	
	//first corner
	checkTilePosition(levelData, collideTilePosition, m_position.x, m_position.y);
	// second corner
	checkTilePosition(levelData, collideTilePosition, m_position.x + AGENT_WIDTH, m_position.y);
	// third corner
	checkTilePosition(levelData, collideTilePosition, m_position.x, m_position.y + AGENT_WIDTH);
	// fourth corner
	checkTilePosition(levelData, collideTilePosition, m_position.x + AGENT_WIDTH, m_position.y + AGENT_WIDTH);

	if (collideTilePosition.size() == 0) {
		return false;
	}


	// do collition
	for (int i = 0; i < collideTilePosition.size(); i++) {
		collideWithTile(collideTilePosition[i]);
	}
	return true;
}

bool Agent::collideWithAgent(Agent* agent)
{
	float AGENT_RADIUS = 60.0f;

	const float MIN_DISTANCE = m_radius + agent->getRadius();

	glm::vec2 PointA = m_position + glm::vec2(m_radius);
	glm::vec2 PointB = agent->getPosition() + glm::vec2(agent->getRadius());

	glm::vec2 disVector = PointA - PointB;

	float distance = glm::length(disVector);

	float collisionDistance = MIN_DISTANCE - distance;
	if (collisionDistance > 0) {

		glm::vec2 collisionDistanceVec = glm::normalize(disVector) * collisionDistance;

		m_position += collisionDistanceVec / 2.0f;
		agent->m_position -= collisionDistanceVec / 2.0f;
		return true;
	}
	return false;
}

void Agent::checkTilePosition(const std::vector<std::string>& levelData,
	std::vector<glm::vec2>& collideTilePosition,
	float x,
	float y)
{
	glm::vec2 cornerPos = glm::vec2(floor(x / (float)TILE_WIDTH),
										floor(y / (float)TILE_WIDTH));

	// if outside world return 
	if (cornerPos.x < 0 || cornerPos.x >= levelData[0].size() ||
		cornerPos.y < 0 || cornerPos.y >= levelData.size()) {
		return;
	}
	
		if (levelData[cornerPos.y][cornerPos.x] != '.') {
		collideTilePosition.push_back(cornerPos * (float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH / 2.0f));
	}

}

// AABB collision 
void Agent::collideWithTile(glm::vec2 tilePos)
{
	const float AGENT_RADIUS = (float)AGENT_WIDTH / 2.0f;
	const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
	const float MIN_DISTANCE = m_radius + TILE_RADIUS;

	glm::vec2 centerPlayerPos = m_position + glm::vec2(m_radius);
	glm::vec2 disVect = centerPlayerPos - tilePos;
	
	float xDepth = MIN_DISTANCE - abs(disVect.x);
	float yDepth = MIN_DISTANCE - abs(disVect.y);

	if (xDepth > 0 || yDepth > 0) { // if true then there is a coliision
		if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)) {
			if (disVect.x < 0) {
				m_position.x -= xDepth;	// push out of block by xdepth
			}
			else {
				m_position.x += xDepth;	// push out of block by xdepth
			}
		}
		else {
			if (disVect.y < 0) {
				m_position.y -= yDepth;	// push out of block by xdepth
			}
			else {
				m_position.y += yDepth;	// push out of block by xdepth
			}
		}
	}
}
