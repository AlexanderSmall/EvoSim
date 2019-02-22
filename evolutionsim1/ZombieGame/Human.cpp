#include "Human.h"

#include <odingine/ResourceManager.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp> 

#include <random>
#include <ctime>

Human::Human() : m_frames(0)
{
}


Human::~Human()
{
}

void Human::init(float speed, glm::vec2 pos)
{
	static std::mt19937 randEng(time(nullptr));
	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

	m_health = 20;

	m_color.r = 200;
	m_color.g = 0;
	m_color.b = 200;
	m_color.a = 255;

	m_speed = speed;
	m_position = pos;
	m_direction = glm::vec2(randDir(randEng), randDir(randEng));

	// incase direction = 0
	if (m_direction.length() == 0) m_direction = glm::vec2(1.0f, 0.0f);

	//normalise direction
	m_direction = glm::normalize(m_direction);

	m_textureID = odingine::ResourceManager::getTexture("Textures/circle.png").id;

}

void Human::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies)
{
	static std::mt19937 randEng(time(nullptr));
	static std::uniform_real_distribution<float> randRotate(-5.0f, 5.0f);

	m_position += m_direction * m_speed;

	// human randomly changes direction every 20 frames
	if (m_frames == 120) {
		m_direction = glm::rotate(m_direction, randRotate(randEng));
		m_frames = 0;
	}
	else {
		m_frames++;
	}

	// if agent collision with wall - change direction
	if (CollideWithLevel(levelData)) {
		m_direction = glm::rotate(m_direction, randRotate(randEng));
	}

}
