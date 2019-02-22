#include "Zombie.h"
#include "Human.h"


Zombie::Zombie()
{
}


Zombie::~Zombie()
{
}

void Zombie::init(float speed, glm::vec2 pos)
{
	m_speed = speed;
	m_position = pos;
	m_health = 50;

	m_color = odingine::ColorRGBA8(30, 220, 20, 255);
}

void Zombie::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies)
{

	Human* closestHuman = getClosestHuman(humans);

	if (closestHuman != nullptr){
		glm::vec2 direction = glm::normalize(closestHuman->getPosition() - m_position);
		m_position += direction * m_speed;
	}


	CollideWithLevel(levelData);
}

Human* Zombie::getClosestHuman(std::vector<Human*>& humans)
{
	Human* closestHuman = nullptr;
	float distanceSmallest = 9999999999.0f;

	for (int i = 0; i < humans.size(); i++) {
		glm::vec2 disVector = humans[i]->getPosition() - m_position;
		float distance = glm::length(disVector);

		if (distance < distanceSmallest) {	// if true then this is the closest human
			distanceSmallest = distance;
			closestHuman = humans[i];
		}
	}

	return closestHuman;
}
