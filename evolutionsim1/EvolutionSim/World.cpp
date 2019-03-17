#pragma once
#include "World.h"
#include "Terrain.h"
#include "GeneticAlgorithm.h"

#include <iostream>
#include <stdlib.h>
#include <Box2D/Box2D.h>

#include <math.h>

constexpr char const* const pPositionAttribName = "position";
constexpr char const* const pColourAttribName = "colour";

const int GRAVITY = -9.81;

std::default_random_engine generator;
int fertileSize = (int)NUM_AGENT_POP * 0.5;	// calculate and cast fertileSize to int
std::discrete_distribution<int> CullDistribution{ 0,1,2,2,3,3,4,4,5,5 };
std::discrete_distribution<int> MateSelectionDistrobution{ 4,4,4,3,3,3,3,2,1,1 };

// create initialisation lists



World::World(odingine::Window* window)  
{
	m_b2world = nullptr;

	m_window = window;
	// INIT SHADERS
	initShaders();
	initWorld();
}

void World::initWorld()
{
	// Set up scene for rendering.
	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);

	m_debugDraw = new b2draw::DebugDraw(glGetAttribLocation(m_colorProgram.getProgramID(), pPositionAttribName),
		glGetAttribLocation(m_colorProgram.getProgramID(), pColourAttribName), 16, 0.f);

	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	//flags += b2Draw::e_aabbBit;
	flags += b2Draw::e_centerOfMassBit;
	m_debugDraw->SetFlags(flags);
	std::cout << m_debugDraw->GetFlags() << " - flags" << std::endl;

	createWorld();

	initCamera();

	createTerrain();

	//randomMethod();

	createAgents();


}

void World::randomMethod() {	// method to test distrobution of random variables
	
	int distrobution[10];

	for (int i = 0; i < 10; i++) {
		distrobution[i] = 0;
	}

	int rand;
	for (int i = 0; i < 50; i++) {
		rand = MateSelectionDistrobution(generator);
		distrobution[rand] += 1;
	}

	for (int i = 0; i < 10; i++) {
		std::cout << i << ": " << distrobution[i] << std::endl;
	} 

}

void World::initCamera()
{	
	// Init camera
	m_camera.init(m_screenWidth, m_screenHeight);
	m_camera.setScale(7.0f);
	// test for camera
	m_camera.setPosition(glm::vec2(0, 0));
	m_camera.update();
	
}

void World::createWorld() {
	// Create the world
	b2Vec2 const gravity(0.0f, GRAVITY);
	m_b2world = new b2World(gravity);

	m_b2world->SetDebugDraw(m_debugDraw);	// init debugdraw

}

void World::createAgents()
{
	int x = 0;
	int y = 0;

	for (int i = 0; i < NUM_AGENT_POP; i++) {
		spawnAgent(++m_agentCount, 0, 0);
	}
}

void World::createTerrain() {
	m_terrain = new Terrain(m_b2world);
	//m_terrain->CreateHillTerrain();
	m_terrain->CreateRoughTerrain();
	//m_terrain->CreateFlatTerrain();
}


void World::setCameraPosition(glm::vec2 position)
{
	m_camera.setPosition(position); // set camera position
	m_camera.update();
}

void World::initShaders() {
	// Compile our color shader
	m_colorProgram.compileShaders("Shaders/vertexShader.vs", "Shaders/fragmentShader.fs");
	m_colorProgram.linkShaders();
}

void logBodies(b2World const* pWorld)
{
	for (
		b2Body const* pBody = pWorld->GetBodyList();
		pBody != nullptr;
		pBody = pBody->GetNext()
		)
	{
		auto bodynum = pBody->GetFixtureList()->GetShape();
		auto const pos = pBody->GetPosition();
		float32 const angle = pBody->GetAngle();
		std::cout << "Body: " << bodynum << " at - (" << pos.x << ", " << pos.y << ") @ " << angle
			<< std::endl;
	}
}

void World::FollowBestAgent()
{
	// follow agent best agent
	if (m_agents.size() > 0) {

		int bestAgentScore = 0;
		int bestAgentID = 0;
		for (int i = 0; i < m_agents.size(); i++) {
			int score = m_agents[i]->getPosition().x;
			if (score > bestAgentScore) {
				bestAgentID = i;
				bestAgentScore = score;
			}

		}

		int x = m_agents[bestAgentID]->getPosition().x;
		int y = m_agents[bestAgentID]->getPosition().y;

		m_camera.setPosition(glm::vec2(x, y));
		m_camera.update();
	}
}

void World::update()
{
	//m_camera.update();	// maybe remove
	m_b2world->Step(1.0f / 60.0f, 6, 2);
	m_b2world->ClearForces();
	m_debugDraw->Clear();

	FollowBestAgent();


	m_b2world->DrawDebugData();	//

	m_debugDraw->BufferData();	// 
	//logBodies(m_b2world);
}

void World::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_colorProgram.use();
	
	glUseProgram(m_colorProgram.getProgramID());
	//glUniformMatrix4fv(m_mvpAttribLoc, 1, GL_FALSE, m_pMvpMatStart);
	
	//get camera matrix
	GLint pUniform = m_colorProgram.getUniformLocation("MVP");
	glm::mat4 projectionMatrix = m_camera.getCameraMatrix();

	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);
	
	m_debugDraw->Render();

	m_colorProgram.unuse();

	m_window->swapBuffer();
}


void World::spawnAgent(int ID, int x, int y) 
{ 
	//std::cout << "NEW AGENT" << std::endl;
	// generate Genetic material for agent
	GeneticAlgorithm* m_GA = new GeneticAlgorithm();
	m_GA->generateChromosomes();

	//std::cout << "...... \n ....... \n ...... \n" << std::endl;
	//m_GA->outputAgentChromes();
	//m_GA->FlattenChrome();

	// generate agents
	Agent* agent = new Agent(m_GA, m_b2world, ID, x, y);
	m_agents.emplace_back(agent);

	// may only need to keep GA to recreated agent	
}

void World::spawnAgent(GeneticAlgorithm* GA, int ID, int x, int y)	// spawn agent with GA chromes
{
	// generate agents given existing Genetic code
	Agent* agent = new Agent(GA, m_b2world, ID, x, y);
	m_agents.emplace_back(agent);
}

void World::clearAgents() 
{
	//for (int i = 0; i < m_agents.size(); i++) {
	//	m_b2world->DestroyBody(m_agents[i]->getAgentBody());
	//}

	//m_b2world->DestroyBody(m_b2world->GetBodyList());

	//std::cout << "body list: " << m_b2world->GetBodyList() << std::endl;
	//std::cout << "body count: " << m_b2world->GetBodyCount() << std::endl;
		
	for (int i = 0; i < m_b2world->GetJointCount(); i++) {
		m_b2world->DestroyJoint(m_b2world->GetJointList());
		m_b2world->GetJointList()->GetNext();
	}

	for (int i = 0; i < m_b2world->GetBodyCount(); i++) {	// destroy all bodies in the world

		for (b2Fixture* f = m_b2world->GetBodyList()->GetFixtureList(); f;) {
			b2Fixture* ftd = f;
			f = f->GetNext();
			m_b2world->GetBodyList()->DestroyFixture(ftd);
		}

		m_b2world->DestroyBody(m_b2world->GetBodyList());
		m_b2world->GetBodyList()->GetNext();
	}


	m_agents.clear();

	//std::cout << "body list2: " << m_b2world->GetBodyList() << std::endl;
	//std::cout << "body count2: " << m_b2world->GetBodyCount() << std::endl;

	//std::cout << "start of error: " << std::endl;
	//for (int i = 0; i < m_b2world->GetBodyCount(); i++) {
		//std::cout << "body" << i << m_b2world->GetBodyList() << std::endl;
		//m_b2world->GetBodyList()->GetNext();
	//}



}

void World::OutputAgentChromes()
{
	for (int i = 0; i < m_agents.size(); i++) {
		std::cout << "Agent " << i << std::endl;
		m_agents[i]->getGA()->FlattenChrome();
	}
}

void World::OutputAgentID()
{
	for (int i = 0; i < m_agents.size(); i++) {
		std::cout << "Position [" << i << "] - Agent:" << m_agents[i]->getID() << " Distance x: " << m_agents[i]->getPosition().x << std::endl;
	}
}

void World::CalculateStatistics()
{
	// calculate mean x distance of entire population - plot of graph
	// calculate Q1 and Q3 of population - plot on graph

	float totalDistance = 0;
	float bestAgentDistance = 0;
	for (int i = 0; i < m_agents.size(); i++) {
		int distance = m_agents[i]->getPosition().x;
		totalDistance += distance;
		if (distance > bestAgentDistance) {
			bestAgentDistance = distance;
		}
	}
	float mean = totalDistance / NUM_AGENT_POP;


	std::cout << "EPOCH: " << m_currentEpoch << std::endl;
	std::cout << "BEST AGENT: " << bestAgentDistance << std::endl;
	std::cout << "MEAN: " << mean << std::endl;

	int medianIndex = (int) NUM_AGENT_POP / 2;
	std::cout << "MEDIAN: " << m_agents[medianIndex]->getPosition().x << std::endl;

}

void World::incrementAgentAge()
{
	for (int i = 0; i < m_agents.size(); i++) {
		m_agents[i]->incrementAge();
	}
}

void World::sortAgents()
{
	// simple bubble sort to order agents based on x position
	for (int i = 0; i < m_agents.size() - 1; i++) {
		for (int j = 0; j < m_agents.size() - i - 1; j++) {
			if (m_agents[j]->getPosition().x < m_agents[j + 1]->getPosition().x) {
				//swap(m_agents[j], m_agents[j + 1]);
				Agent* temp = m_agents[j];
				m_agents[j] = m_agents[j + 1];
				m_agents[j + 1] = temp;
			}
		}
	}

	// print sorted list
	for (int i = 0; i < m_agents.size(); i++) {
		//std::cout << "x pos: " << m_agents[i]->getPosition().x << std::endl;
	}



}

void World::SelectFertileAgents() 
{
	// order agents based on x position	
	sortAgents();

	OutputAgentID();

	CalculateStatistics();	// calculate statistics on population

	//std::cout << "agent size before: " << m_agents.size() << std::endl;

	for (int i = 0; i < m_agents.size(); i++) {	
		m_fertileAgents[i] = m_agents[i];	// transfer agent data to array datatype
	}
	clearAgents();	// clear the existing agents from m_agents and m_b2world
	clearAgents();
	clearAgents();
	clearAgents();
	clearAgents();

	// kill 50 agents (1/2 of agents)
	// if agent age over x amount then destroy first
	for (int i = 0; i < NUM_AGENT_POP; i++) {
		if (m_fertileAgents[i]->getEpochAge() > m_maxAge) {
			//m_agents.erase(m_agents.begin() + i);
			m_fertileAgents[i] = NULL;
		}
	}

	//std::cout << "new agent size cull 1: " << m_agents.size() << std::endl;

	// Cull more agents at randomly until 50 reached (higher probability to remove bad agents)
	GeneticAlgorithm* g = new GeneticAlgorithm();
	int offset = 0;
	int index = 0;
	float rand = 0;
	int numAgents = NUM_AGENT_POP;
	while (numAgents > NUM_AGENT_POP * CULL_PERCENTAGE) {		// 100 * 0.5 = 50 (cull 50% of populations)	
		//rand = g->generateRandomInt(0, NUM_AGENT_POP);
		//m_agents.erase(m_agents.begin() + rand);

		offset = CullDistribution(generator);
		//std::cout << "offset: " << offset <<  " - index:" << ((offset + 1) * (NUM_AGENT_POP / 10) - g->generateRandomInt(1, NUM_AGENT_POP / 10)) << std::endl;
		index = (offset) * (NUM_AGENT_POP / 10) + g->generateRandomInt(0, (NUM_AGENT_POP / 10) - 1);
		if (m_fertileAgents[index] != NULL) {
			m_fertileAgents[index] = NULL; // divide by 10 because the size of distrobution
			//std::cout << "REMOVED " << index << std::endl;
			numAgents--;
		}
		else {
			//std::cout << index << ": " << "ALREADY NULL:" << std::endl;
		}
	}

	//m_fertileAgents = m_agents;	// swap fertile agents to new vector

	//std::cout << "new agent size cull 2: " << m_fertileAgents.size() << std::endl;

	// print fertile agenst
	//std::cout << "START OF CULLING" << std::endl;
	int count = 0;
	for (int i = 0; i < NUM_AGENT_POP; i++) {	// map culled agents to new array
		//std::cout << "agent" << i << std::endl;
		if (m_fertileAgents[i] != NULL) {
			//std::cout << "agent " << i << ": " << m_fertileAgents[i]->getAgentBody() << std::endl;
			m_fertileAgentsSelection[count] = m_fertileAgents[i];
			count += 1;
		}
		else {
			//std::cout << i << ": " << "NULL" << std::endl;
		}
	}
	//std::cout << "END OF CULLING" << std::endl;

	//for (int i = 0; i < (NUM_AGENT_POP * CULL_PERCENTAGE); i++) {	// map culled agents to new array
	//	std::cout << "agent " << i << ": " << m_fertileAgentsSelection[i]->getAgentBody() << std::endl;
	//}

	SelectAgentPartners();

}

void World::SelectAgentPartners() 	// takes list of fertile agents selects pairs to breed
{
	// select which agents will mate with each other
	// agents with better fittness scores will be more likely to have offspring

	GeneticAlgorithm* g = new GeneticAlgorithm();
	
	
	//std::cout << "SAMPLE START: " << std::endl;
	//int sampleSize = (int) NUM_AGENT_POP * CULL_PERCENTAGE;
	//int sampleSize = 50;
	//for (int i = 0; i < NUM_AGENT_POP * CULL_PERCENTAGE; i++) {
	int newAgents = 0;
	while (newAgents < NUM_AGENT_POP * CULL_PERCENTAGE) {
		int offset = MateSelectionDistrobution(generator);
		int index = (offset) * ((NUM_AGENT_POP / 10) * CULL_PERCENTAGE) + g->generateRandomInt(1, (NUM_AGENT_POP / 10)  * CULL_PERCENTAGE);
		
		int offset2 = MateSelectionDistrobution(generator);
		int index2 = (offset2) * ((NUM_AGENT_POP / 10) * CULL_PERCENTAGE) + g->generateRandomInt(0, (NUM_AGENT_POP / 10)  * CULL_PERCENTAGE);

		//std::cout << "index1: " << index << std::endl;
		//std::cout << "index2: " << index2 << std::endl;

		if (index != index2) {
			GeneticAlgorithm* newGenome = CrossoverAgent(m_fertileAgentsSelection[offset], m_fertileAgentsSelection[offset2]); // cross over two agents
			spawnAgent(newGenome, ++m_agentCount, 0, 0);	// spawn the new crossover agent
			newAgents++;
		}
	}

	CreateSurvivingAgents();				// spawn the agents that survived

	// displays all fertile agents
	//for (int i = 0; i < (NUM_AGENT_POP * CULL_PERCENTAGE); i++) {	// map culled agents to new array
	//	std::cout << "agent " << i << ": " << m_fertileAgentsSelection[i]->getAgentBody() << std::endl;
	//}

}

void World::CreateSurvivingAgents()
{
	for (int i = 0; i < NUM_AGENT_POP * CULL_PERCENTAGE; i++) {
		GeneticAlgorithm* GA = m_fertileAgentsSelection[i]->getGA();
		spawnAgent(GA, m_fertileAgentsSelection[i]->getID(), 0, 0);
	}
}

void World::MutateGenome()
{

}

GeneticAlgorithm* World::CrossoverAgent(Agent* a1, Agent* a2)
{
	// takes two agents and crosses their genome
	
	GeneticAlgorithm* g = new GeneticAlgorithm();
	int crossoverSize = g->generateRandomInt(1, a1->getGA()->getSymmetrySize() - 1);	// crossover size is where the crossover occurs in agent chromes

	//std::cout << "split index: " << crossoverSize << std::endl;

	GeneticAlgorithm* newGenome = new GeneticAlgorithm();			// create new genome
	
	AgentChromes agentChromes1 = a1->getGA()->getAgentChromes();	// get chromes from agent 1
	AgentChromes agentChromes2 = a2->getGA()->getAgentChromes();	// get chromes from agent 2
	
	for (int i = 0; i < crossoverSize; i++) {						// add first half of agent 1 chromes to new genome
		newGenome->addAgentChromes(agentChromes1.lines[i]);			
	}

	for (int i = a2->getGA()->getSymmetrySize(); i > crossoverSize; i--) { // add second half of agent 2 chromes to new genome
		newGenome->addAgentChromes(agentChromes2.lines[i - 1]);
	}

	/*
	std::cout << "GENOME CROSSOVER START" << std::endl;
	std::cout << "chrome 1" << std::endl;
	a1->getGA()->FlattenChrome();
	std::cout << "chrome 2" << std::endl;
	a2->getGA()->FlattenChrome();
	std::cout << "new chrome" << std::endl;
	newGenome->FlattenChrome();
	std::cout << "GENOME CROSSOVER END" << std::endl;
	*/

	newGenome->SearchAgentChromes(); // mutate random gene with mutation given mutation rate

	return newGenome;		// return new genome



}

void World::EvolveAgents() 
{
	SelectFertileAgents();
}





