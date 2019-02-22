#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "Bullet.h"

class Weapon
{
public:
	Weapon(std::string name, int fireRate, int numBulletsInShot, float spread, float projectileSpeed, float projectileDamage);
	~Weapon();

	void update(bool isMouseDown, const glm::vec2& weaponPos, glm::vec2 direction, std::vector<Bullet>& bullets);

private:

	void fire(const glm::vec2& direction, const glm::vec2& weaponPos, std::vector<Bullet>& bullets);

	std::string m_name;

	int m_fireRate; // fire rate in fps

	int m_numBulletsInShot; // number of bullets in one shot

	float m_spread; // accuracy

	float m_projectileSpeed;	// projectile speed

	float m_projectileDamage;

	int m_frameCounter;

};

