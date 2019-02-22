#include "Weapon.h"

#include<random>
#include<ctime>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp> 
#include <glm/glm.hpp>

Weapon::Weapon(std::string name, int fireRate, int numBulletsInShot, float spread, float projectileSpeed, float projectileDamage) :
	m_name(name), m_fireRate(fireRate),
	m_numBulletsInShot(numBulletsInShot),
	m_spread(spread),
	m_projectileSpeed(projectileSpeed),
	m_projectileDamage(projectileDamage),
	m_frameCounter(0)
{
}

Weapon::~Weapon()
{
}

void Weapon::update(bool isMouseDown, const glm::vec2& weaponPos, glm::vec2 direction, std::vector<Bullet>& bullets)
{
	// limit firerate using frames
	m_frameCounter++;	// count frames
	if (m_frameCounter >= m_fireRate && isMouseDown) {
		fire(direction, weaponPos, bullets);
		m_frameCounter = 0;
	}
}

void Weapon::fire(const glm::vec2& direction, const glm::vec2& weaponPos, std::vector<Bullet>& bullets)
{
	static std::mt19937 randEng(time(nullptr));
	std::uniform_real_distribution<float> randRotate(-m_spread, m_spread);

	for (int i = 0; i < m_numBulletsInShot; i++) {

		bullets.emplace_back(weaponPos, glm::rotate(direction, randRotate(randEng)), m_projectileDamage, m_projectileSpeed);
	}
}
