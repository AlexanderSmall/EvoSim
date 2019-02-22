#pragma once
#include <glm/glm.hpp>

#include <odingine/SpriteBatch.h>

class Bullet
{
public:
	Bullet(glm::vec2 pos, glm::vec2 dir, float speed, int lifeTime);
	~Bullet();

	void draw(odingine::SpriteBatch& spriteBatch);

	bool update();	// returns true if bullet should be destroyed

private:
	int _lifeTime;
	float _speed;
	glm::vec2 _direction;
	glm::vec2 _position;

	odingine::SpriteBatch _spriteBatch;
};

