#include "Terrain.h"
#include "GeneticAlgorithm.h"

#include <random>
#include <ctime>

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

	edgeShape.Set(b2Vec2(-30, 0), b2Vec2(-10, -10));
	m_terrainVertices.push_back(edgeShape);

	groundBody->CreateFixture(&myFixtureDef); 
	b2Vec2 left = b2Vec2(-10, -10);
	for (int i = 0; i < 1000; i++) {
		b2Vec2 right = b2Vec2(ga->generateRandomInt(8, 15) + left.x + sin(i / 10), left.y + (ga->generateRandomFloat(0.5f, 10.0f) * sin(i / 2)));
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
	for (int i = 0; i < 1000; i++) {
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
	for (int i = 0; i < 1000; i++) {
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