#pragma once

#include <odingine/Vertex.h>

#include <Box2D/Box2D.h>
#include <glm/glm.hpp>
//#include <odingine/Vertex.h>

#include <vector>

#include "GeneticAlgorithm.h"

class Agent {
public:

	Agent(GeneticAlgorithm* GA, b2World* b2world, int x, int y);
	~Agent();

	void init(b2BodyDef &bodyDef);

	//void init(b2World* world, const glm::vec2& position, const std::vector<b2PolygonShape*> &shapes, odingine::ColorRGBA8 color);
	const  odingine::ColorRGBA8& getColor() const { return m_color; }

	b2Vec2 getPosition() { return m_agentBody->GetPosition(); }
	void drawAgent();

	// getters
	static const int getMAX_AGENT_BODY_EDGES() { return MAX_AGENT_BODY_EDGES; }	// REMOVE

	std::vector<b2Body*> getAgentParts() { return m_agentParts; }

	b2Body* getAgentBody() { return m_agentBody; }

private:

	// world variables
	b2World* m_b2world;
	GeneticAlgorithm* m_GA;				 // Genome


	// Agent variables
	int m_agentID;
	//glm::vec2 m_position;				 // position of body	
	odingine::ColorRGBA8 m_color;		 // colour

	// Box2D Agent variables
	b2Body* m_agentBody = nullptr;
	std::vector<b2Body*> m_agentParts;


};

