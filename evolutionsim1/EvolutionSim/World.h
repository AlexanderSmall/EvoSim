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

//static const int NUM_AGENT_POP = 10;

class World
{
public:
	World(odingine::Window* window);

	~World();

	void initWorld();
	void initCamera();

	void createWorld();

	void updateWorld();

	void createAgentsTwo();

	void setCameraPosition(glm::vec2 position);

	void initShaders();

	b2World* getWorld() { return m_b2world;  }
	//std::vector<Agent*> getAgents() { return m_agents;	 } 

	void compute();

	void update();

	void render();

	void drawGame();

	void sortAgentParts();


	// setters
	void spawnAgent(int x, int y);

	//std::map<std::string, std::function<void()>> actions;

private:

	void createAgents();

	const float GRAVITY = -15.0f;
	static const int ITERATIONS = 30;

	b2World* m_b2world;
	b2Body* m_groundBody;	// ground body
	b2PolygonShape m_groundBox; // ground box - temp

	odingine::GLSLProgram m_colorProgram; ///< The shader program
	
	odingine::Camera2D m_camera; ///< Main Camera


	const int m_screenWidth = 1024;
	const int m_screenHeight = 500;
	
	odingine::Window* m_window; ///< The game window

	b2draw::DebugDraw* m_debugDraw;

	// Render& i(Render&); // what does this do?
	//Render* m_renderer


	// Agents
	std::vector<Agent*> m_agents;

	// Terrain
	Terrain* m_terrain;

	// GeneticAlgorithm
	GeneticAlgorithm* m_GA;


	// contact listener
	//ContactListener *m_contactListener;

	Agent* agent;	// temp agent to hold value during creation

	bool should_regenerate = false;

	float* m_pMvpMatStart;

	GLint m_mvpAttribLoc;

	int m_clickX;
	int m_clickY;

};

