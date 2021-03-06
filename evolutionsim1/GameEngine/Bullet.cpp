#include "Bullet.h"
#include <odingine/ResourceManager.h>
#include <odingine/GLTexture.h>


Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float speed, int lifeTime)
{
	_position = pos;
	_direction = dir;
	_speed = speed;
	_lifeTime = lifeTime;
}


Bullet::~Bullet()
{
}

void Bullet::draw(odingine::SpriteBatch& spriteBatch)
{
	odingine::Color color;
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static odingine::GLTexture texture = odingine::ResourceManager::getTexture("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	glm::vec4 posAndSize = glm::vec4(_position.x, _position.y, 30, 30);

	spriteBatch.draw(posAndSize, uv, texture.id, 0.0f, color);
}

bool Bullet::update()
{
	_position += _direction * _speed;
	_lifeTime--;
	if (_lifeTime == 0) {
		return true;
	}
	return false;

}
