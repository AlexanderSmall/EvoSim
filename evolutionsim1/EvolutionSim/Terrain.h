#pragma once
#include <Box2D/Box2D.h>

#include <vector>

class Terrain
{
public:
	Terrain(b2World* b2world);
	~Terrain();

	void CreateHillTerrain();

	void CreateRoughTerrain();

	void CreateFlatTerrain();

	void CreateWaterTerrain();

	void saveTerrain();

	void loadTerrain();

private:
	b2World* m_b2world;

	std::vector<b2EdgeShape> m_terrainVertices;

};

