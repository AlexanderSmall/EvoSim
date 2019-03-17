#pragma once

#include <odingine/Vertex.h>

#include <Box2D/Box2D.h>
#include <glm/glm.hpp>
//#include <odingine/Vertex.h>

#include <vector>

#include "GeneticAlgorithm.h"

class Agent {
public:

	Agent(GeneticAlgorithm* GA, b2World* b2world, int ID, int x, int y);
	~Agent();

	void init(b2BodyDef &bodyDef);

	//void init(b2World* world, const glm::vec2& position, const std::vector<b2PolygonShape*> &shapes, odingine::ColorRGBA8 color);
	const  odingine::ColorRGBA8& getColor() const { return m_color; }

	b2Vec2 getPosition() { return m_agentBody->GetPosition(); }


	// getters
	static const int getMAX_AGENT_BODY_EDGES() { return MAX_AGENT_BODY_EDGES; }	// REMOVE

	std::vector<b2Body*> getAgentParts() { return m_agentParts; }

	b2Body* getAgentBody() { return m_agentBody; }

	int getEpochAge() { return m_epochAge;  }

	void incrementAge() { m_epochAge++;}

	GeneticAlgorithm* getGA() { return m_GA; }

	void setGA(GeneticAlgorithm* GA);

	int getID() { return m_ID; }
	void setID(int ID) { this->m_ID = ID; };

	int totalAgents;
	int getTotalAgents() { return totalAgents; }

private:

	int m_ID;

	// world variables
	b2World* m_b2world;

	// Agent variables
	int m_agentID;						 // agent ID
	GeneticAlgorithm* m_GA;				 // Genome
	odingine::ColorRGBA8 m_color;		 // colour
	int m_epochAge = 0;

	// Box2D Agent variables
	b2Body* m_agentBody = nullptr;
	std::vector<b2Body*> m_agentParts;


};

