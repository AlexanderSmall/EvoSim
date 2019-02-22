#include "Level.h"

#include <odingine/Errors.h>
#include <odingine/ResourceManager.h>

#include <fstream>
#include <iostream> // REMOVE

Level::Level(const std::string& fileName)
{

	std::ifstream file;
	file.open(fileName);
	if (file.fail()) {	// error check
		odingine::fatalError("Failed to open " + fileName);
	}

	std::string tmp;
	file >> tmp >> m_numHumans;	// throw away first string and read number of humans
	
	// remove first line
	std::getline(file, tmp);

	// Read level data and add to level data
	while (std::getline(file, tmp)) {
		m_levelData.push_back(tmp);
	}

	m_spriteBatch.init();

	// test print level data
	//for (int i = 0; i < m_levelData.size(); i++) {
	//	std::cout << m_levelData[i] << std::endl;
	//}

	m_spriteBatch.begin();

	glm::vec4 uvRect(1.0f, 1.0f, 1.0f, 1.0f);	// set up uv rect
	odingine::ColorRGBA8 whiteColor;
	whiteColor.r = 255;
	whiteColor.g = 255;
	whiteColor.b = 255;
	whiteColor.a = 255;

	for (int y = 1; y < m_levelData.size(); y++) { // Y 
		for (int x = 0; x < m_levelData.size(); x++) {
			char tile = m_levelData[y][x];	// get tile

			glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);	// get dest rect

			switch (tile) { // select tile type
				case 'B':
				case 'R':
					m_spriteBatch.draw(destRect,
						uvRect,
						odingine::ResourceManager::getTexture("Textures/red_bricks.png").id,
						0.0f,
						whiteColor);
					break;

				case 'L':
					m_spriteBatch.draw(destRect,
						uvRect,
						odingine::ResourceManager::getTexture("Textures/light_bricks.png").id,
						0.0f,
						whiteColor);
					break;
				case 'G':
					m_spriteBatch.draw(destRect,
						uvRect,
						odingine::ResourceManager::getTexture("Textures/glass.png").id,
						0.0f,
						whiteColor);
					break;
				case '.' :
					break;
				case '@':
					m_levelData[y][x] = '.';	// remove player position from map to avoid collision
					m_startPlayerPos.x = x * TILE_WIDTH;
					m_startPlayerPos.y = y * TILE_WIDTH;
					break;
				case 'Z':
					m_levelData[y][x] = '.';	// remove zombie position from map to avoid collision
					m_startZombiePos.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
					break;
				default:
					std::printf("unexpected symbol %c at : (%d, %d)", tile, x, y);
					break;
			}
		}
	}

	m_spriteBatch.end();

}

Level::~Level() 
{
}

void Level::draw() {
	m_spriteBatch.renderBatch();
}
