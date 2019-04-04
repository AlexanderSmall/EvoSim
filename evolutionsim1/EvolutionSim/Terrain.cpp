#include "Terrain.h"
#include "GeneticAlgorithm.h"

#include <random>
#include <ctime>
#include <iostream>
#include <string>
#include <fstream>

Terrain::Terrain(b2World* b2world)
{
	m_b2world = b2world;
}


Terrain::~Terrain()
{

}

void Terrain::CreateHillTerrain() {
	//body def
	b2BodyDef myBodyDef;
	myBodyDef.type = b2_dynamicBody;

	//fixture def
	b2FixtureDef myFixtureDef;
	myFixtureDef.density = 1;

	//body to support the 'ground' fixtures
	myBodyDef.position.Set(0, 0);
	b2Body* fakeGroundBody = m_b2world->CreateBody(&myBodyDef);

	//ground made of edges
	b2EdgeShape edgeShape;

	//ground made of edges with ghost vertices set
	myFixtureDef.shape = &edgeShape;
	myFixtureDef.friction = 1.0f;
	//ground body
	b2BodyDef groundBodyDef;
	b2Body* groundBody = m_b2world->CreateBody(&groundBodyDef);

	GeneticAlgorithm* ga = new GeneticAlgorithm();

	edgeShape.Set(b2Vec2(-60, 0), b2Vec2(-30, -30));
	m_terrainVertices.push_back(edgeShape);

	groundBody->CreateFixture(&myFixtureDef); 
	b2Vec2 left = b2Vec2(-30, -30);
	for (int i = 0; i < 1000; i++) {
		b2Vec2 right = b2Vec2(ga->generateRandomInt(3.0f, 3.0f) + left.x + sin(i / 8), left.y + (ga->generateRandomFloat(3.0f, 3.0f) * cos(i / 3)));
		edgeShape.Set(left, right);
		m_terrainVertices.push_back(edgeShape);
		groundBody->CreateFixture(&myFixtureDef);
		left = right;
	}
}

void Terrain::CreateRoughTerrain()
{
	//body def
	b2BodyDef myBodyDef;
	myBodyDef.type = b2_dynamicBody;

	//fixture def
	b2FixtureDef myFixtureDef;
	myFixtureDef.density = 1;

	//body to support the 'ground' fixtures
	myBodyDef.position.Set(0, 0);
	b2Body* fakeGroundBody = m_b2world->CreateBody(&myBodyDef);

	//ground made of edges
	b2EdgeShape edgeShape;

	//ground made of edges with ghost vertices set
	myFixtureDef.shape = &edgeShape;
	myFixtureDef.friction = 1.0f;
	//ground body
	b2BodyDef groundBodyDef;
	b2Body* groundBody = m_b2world->CreateBody(&groundBodyDef);

	GeneticAlgorithm* ga = new GeneticAlgorithm();

	edgeShape.Set(b2Vec2(-30, 0), b2Vec2(-10, -10));
	m_terrainVertices.push_back(edgeShape);

	groundBody->CreateFixture(&myFixtureDef);
	b2Vec2 left = b2Vec2(-10, -10);
	for (int i = 0; i < 100; i++) {
		b2Vec2 right = b2Vec2(ga->generateRandomFloat(2, 3) + left.x + sin(i * 3), left.y + (ga->generateRandomFloat(0.5f, 6.0f) * sin(i * 2)));
		edgeShape.Set(left, right);
		m_terrainVertices.push_back(edgeShape);
		groundBody->CreateFixture(&myFixtureDef);
		left = right;
	}
}

void Terrain::CreateFlatTerrain()
{
	//body def
	b2BodyDef myBodyDef;
	myBodyDef.type = b2_dynamicBody;

	//fixture def
	b2FixtureDef myFixtureDef;
	myFixtureDef.density = 1;

	//body to support the 'ground' fixtures
	myBodyDef.position.Set(0, 0);
	b2Body* fakeGroundBody = m_b2world->CreateBody(&myBodyDef);

	//ground made of edges
	b2EdgeShape edgeShape;

	//ground made of edges with ghost vertices set
	myFixtureDef.shape = &edgeShape;
	myFixtureDef.friction = 1.0f;
	//ground body
	b2BodyDef groundBodyDef;
	b2Body* groundBody = m_b2world->CreateBody(&groundBodyDef);

	GeneticAlgorithm* ga = new GeneticAlgorithm();

	edgeShape.Set(b2Vec2(-30, 0), b2Vec2(-10, -10));
	m_terrainVertices.push_back(edgeShape);

	groundBody->CreateFixture(&myFixtureDef);
	b2Vec2 left = b2Vec2(-10, -10);
	for (int i = 0; i < 100; i++) {
		b2Vec2 right = b2Vec2(left.x + 10, left.y);
		edgeShape.Set(left, right);
		m_terrainVertices.push_back(edgeShape);
		groundBody->CreateFixture(&myFixtureDef);
		left = right;
	}
}

void Terrain::CreateWaterTerrain()
{

}

void Terrain::saveTerrain()
{
	std::ofstream file;
	file.open("terrainData.txt", std::ofstream::out);
	if (file) {
		for (int i = 0; i < m_terrainVertices.size(); i++)
		{
			file << m_terrainVertices[i].m_vertex1.x << "\n";
			file << m_terrainVertices[i].m_vertex1.y << "\n";
			file << m_terrainVertices[i].m_vertex2.x << "\n";
			file << m_terrainVertices[i].m_vertex2.y << "\n";
		}
		file.close();
	}
	else {
		std::cerr << "unable to open terrain file";
		exit(1);
	}
}

void Terrain::loadTerrain()
{
	//body def
	b2BodyDef myBodyDef;
	myBodyDef.type = b2_dynamicBody;

	//fixture def
	b2FixtureDef myFixtureDef;
	myFixtureDef.density = 1;

	//body to support the 'ground' fixtures
	myBodyDef.position.Set(0, 0);
	b2Body* fakeGroundBody = m_b2world->CreateBody(&myBodyDef);

	//ground made of edges
	b2EdgeShape edgeShape;

	//ground made of edges with ghost vertices set
	myFixtureDef.shape = &edgeShape;
	myFixtureDef.friction = 1.0f;
	//ground body
	b2BodyDef groundBodyDef;
	b2Body* groundBody = m_b2world->CreateBody(&groundBodyDef);

	std::ifstream file;
	file.open("terrainData.txt", std::ofstream::in);
	if (file) {
		//for (int i = 0; i < 100; i++) {
		//while (!file.) {
		for (int i = 0; i < 100; i++) {			// change to while loop that can handle any number of vertices
			float x, y;
			file >> x;
			file >> y;
			b2Vec2 v1(x, y);
			file >> x;
			file >> y;
			b2Vec2 v2(x, y);

			edgeShape.Set(v1, v2);
			m_terrainVertices.push_back(edgeShape);
			groundBody->CreateFixture(&myFixtureDef);
		}
	}
	else {
		std::cerr << "unable to open terrain file";
		exit(1);
	}
}
