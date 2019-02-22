#pragma once

#include <odingine/SpriteBatch.h>

#include <string>
#include <vector>

const int TILE_WIDTH = 64;

class Level
{
public:
	// Load the level
	Level(const std::string& fileName);
	~Level();

	void draw();

	//getters
	const std::vector<std::string>& getLevelData() const { return m_levelData;  }

	int getWidth() const { return m_levelData[0].size();  }
	int getHeight() const { return m_levelData.size(); }

	int getNumOfHumans() const { return m_numHumans;  }

	glm::vec2 getStartPlayerPos() const { return m_startPlayerPos; }
	const std::vector<glm::vec2>& getStartZombiePos() const { return m_startZombiePos; }


private:
	std::vector<std::string> m_levelData;
	int m_numHumans;

	odingine::SpriteBatch m_spriteBatch;

	glm::vec2 m_startPlayerPos;
	std::vector<glm::vec2> m_startZombiePos;
};

