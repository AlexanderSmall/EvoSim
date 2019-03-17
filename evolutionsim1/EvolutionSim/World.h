#pragma once

#include <memory>
#include <iostream>
#include <vector>
#include <map>

#include <odingine/GLSLProgram.h>
#include <odingine/Camera2D.h>
#include <odingine/Window.h>

#include <Box2D/Box2D.h>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <SDL/SDL_opengl.h>
#include <GL/glu.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ContactListener.h"
#include "Agent.h"
#include "Terrain.h"
#include "Render.h"

#include "algorithm.h"
#include "DebugDraw.h"
#include "gl.h"

#include <functional>



class World
{
public:
	World(odingine::Window* window);

	~World();

	void initWorld();
	void initCamera();
	void createWorld();

	void setCameraPosition(glm::vec2 position);
	void initShaders();

	b2World* getWorld() { return m_b2world;  }

	void FollowBestAgent();

	void update();
	void render();


	void createAgents();

	void spawnAgent(int ID, int x, int y);
	void spawnAgent(GeneticAlgorithm* GA, int ID, int x, int y);

	void clearAgents();

	void randomMethod();

	void OutputAgentChromes();

	void OutputAgentID();

	// Evolution methods	
	void CalculateStatistics();
	
	void incrementAgentAge();

	void sortAgents();
	void SelectFertileAgents();		// select the Agents for breeding/evolution

	void SelectAgentPartners();

	void CreateSurvivingAgents();
	void MutateGenome();
	GeneticAlgorithm* CrossoverAgent(Agent* a1, Agent* a2);

	void EvolveAgents();			// preform cross-over, mutation

	void incrementEpoch() { m_currentEpoch++; }

	//std::map<std::string, std::function<void()>> actions;

private:

	void createTerrain();

	const float GRAVITY = -15.0f;
	static const int ITERATIONS = 30;

	b2World* m_b2world;

	odingine::GLSLProgram m_colorProgram; ///< The shader program
	
	const int m_screenWidth = 1024;
	const int m_screenHeight = 500;
	odingine::Window* m_window; ///< The game window
	odingine::Camera2D m_camera; ///< Main Camera

	b2draw::DebugDraw* m_debugDraw;

	// Agents
	int m_agentCount = 0;

	static const int NUM_AGENT_POP = 60;	// MINIMUM POPULATION SIZE = 20
	const float MUTATION_RATE = 0.05;
	const float CULL_PERCENTAGE = 0.5;

	int m_currentEpoch = 0;
	int m_maxAge = 20;

	std::vector<Agent*> m_agents;	// holds current population of agents
	static const int cullPop = (int)NUM_AGENT_POP * 0.5;
	Agent* m_fertileAgentsSelection[cullPop];
	Agent* m_fertileAgents[NUM_AGENT_POP];

	// Terrain
	Terrain* m_terrain;

	bool should_regenerate = false;

	int m_clickX;
	int m_clickY;

};

