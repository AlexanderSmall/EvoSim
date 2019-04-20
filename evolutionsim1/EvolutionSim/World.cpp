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
std::discrete_distribution<int> CullDistribution{ 1,2,3,4,5,6,7,8,9,10 };
//std::discrete_distribution<int> CullDistribution{ 1,1,2,2,3,3,3,3,3,3 };
std::discrete_distribution<int> MateSelectionDistrobution{ 4,4,4,3,3,3,3,2,1,1 };

// create initialisation lists



World::World(odingine::Window* window)  
{
	m_b2world = nullptr;
	m_window = window;
	// INIT SHADERS
	initShaders();
	// INIT WORLD
	initWorld();
}

void World::initWorld()
{
	// Set up scene for rendering.
	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);

	m_debugDraw = new b2draw::DebugDraw(glGetAttribLocation(m_colorProgram.getProgramID(), pPositionAttribName),
		glGetAttribLocation(m_colorProgram.getProgramID(), pColourAttribName), 16, 0.f);

	setFlags(true);
	//std::cout << m_debugDraw->GetFlags() << " - flags" << std::endl;

	createWorld();

	initCamera();

	createTerrain();

	distrobutionCheck();

	GenerateChromes();

	// parallel agent - max populationsize of 60
	//createAgents();

	// serial agent - max population of --
	//CreateOneAgent(); // spawn the first agent

}

void World::setFlags(bool toggle) 
{
	if (toggle) {
		uint32 flags = 0;
		flags += b2Draw::e_shapeBit;
		//flags += b2Draw::e_jointBit;
		//flags += b2Draw::e_aabbBit;
		//flags += b2Draw::e_centerOfMassBit;
		m_debugDraw->SetFlags(flags);
	}
	else {
		uint32 flags = 0;
		m_debugDraw->SetFlags(flags);
	}
}

void World::reconstructChromes() // temp method
{
	//m_agents[0]->getGA()->reconstructAgentChromesomes();

	//spawnAgent(m_agents[0]->getGA(), 0, 10, 10);
}

bool World::checkAgentMoving(int pos, float second)
{
	// collect samples
	int num = floor(second) + 1; // + 1 to avoid zero errors
	if (num % samplePos == samplePos - 1) {
		//m_agentPositions.emplace_back(m_agents[pos]->getPosition().x);
		m_agentPositions[samplePos - 1] = m_agents[pos]->getPosition().x;
		
		if (samplePos != 5) {
			samplePos++;
		}
		else {
			samplePos = 1;
		}
	}


	// check they have moved past x = 30 within 5 seconds
	if (floor(second) >= 5) {
		if (m_agents[pos]->getPosition().x < 0) {
			m_agentScores[pos] = -1000;
			return true;
		}

		//calucltae mean of samples
		float sum = 0;
		for (int i = 0; i < 5; i++) {
			sum += m_agentPositions[i];
		}
		float mean = sum / 5;

		// calculate variance
		float varianceTotal = 0;
		for (int i = 0; i < 5; i++) {
			varianceTotal += pow(m_agentPositions[i] - mean, 2);
		}

		float variance;
		variance = varianceTotal / (5 - 1);

		if (variance <= 0.01)
		{
			return true;
		}
		//std::cout << "variance: " << variance << std::endl;
	}
	return false;
}

void World::flattenOne()
{
	m_agents[0]->getGA()->FlattenChrome();
}

void World::distrobutionCheck() 
{	// method to test distrobution of random variables
	
	std::cout << "l" << std::endl;
	std::cout << "16.102411,0.397205,0.827890,11.628681" << std::endl;


	std::cout << "l" << std::endl;
	std::cout << "16.0334185,0.4182115,0.8255545,12.160624" << std::endl;
	std::cout << "j" << std::endl;
	std::cout << "16.0334185,0.4182115,0.8255545,12.160624" << std::endl;
	std::cout << "b" << std::endl;
	std::cout << "l" << std::endl;
	std::cout << "16.0334185,0.4182115,0.8255545,12.160624" << std::endl;
	std::cout << "j" << std::endl;
	std::cout << "16.7218455,0.3767910,0.822672,10.2854565" << std::endl;
	std::cout << "b" << std::endl;

	int distrobution[10];

	for (int i = 0; i < 10; i++) {
		distrobution[i] = 0;
	}

	generator.seed(time(0));		// set seed of generator

	int rand;
	for (int i = 0; i < 50; i++) {
		rand = CullDistribution(generator);
		distrobution[rand] += 1;
	}

	std::cout << "CullDistrobution" << std::endl;
	for (int i = 0; i < 10; i++) {
		std::cout << i << ": " << distrobution[i] << std::endl;
	}

	for (int i = 0; i < 10; i++) {
		distrobution[i] = 0;
	}

	for (int i = 0; i < 50; i++) {
		rand = MateSelectionDistrobution(generator);
		distrobution[rand] += 1;
	}

	std::cout << "MateSelectionDistrobution" << std::endl;
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

void World::CreateOneAgent()
{
	int x = 0;
	int y = 10;

	spawnAgent(++m_agentCount, x, y);
}

void World::createAgents()
{
	int x = 0;
	int y = 10;

	for (int i = 0; i < NUM_AGENT_POP; i++) {
		spawnAgent(++m_agentCount, x, y);
	}
}

void World::RegenerateChromes(int pos)
{
	int agentID = m_agentGenes[pos]->getID();

	GeneticAlgorithm* GA = new GeneticAlgorithm();
	GA->generateChromosomes();
	m_agentGenes[pos] = GA;
	Agent* agent = new Agent(GA, m_b2world, agentID, 0, 10);
}

void World::GenerateChromes()
{
	for (int i = 0; i < NUM_AGENT_POP; i++) {
		GeneticAlgorithm* m_GA = new GeneticAlgorithm();
		m_GA->generateChromosomes();
		m_GA->setID(i);
		m_agentGenes.push_back(m_GA);
		m_agentCount++;
	}
}

void World::addAgentToWorld(int pos)
{
	spawnAgent(m_agentGenes.at(pos), m_agentGenes.at(pos)->getID(), 0, 10);
}

void World::createTerrain() {
	m_terrain = new Terrain(m_b2world);
	//m_terrain->CreateHillTerrain();
	//m_terrain->CreateRoughTerrain();
	m_terrain->CreateFlatTerrain();

	m_terrain->saveTerrain();
}


void World::setCameraPosition(glm::vec2 position)
{
	m_camera.setPosition(position); // set camera position
	m_camera.update();
}

void World::zoomIn()
{
	m_camera.setScale(m_camera.getScale() + 0.1);
}

void World::zoomOut()
{
	m_camera.setScale(m_camera.getScale() - 0.1);
}

void World::moveLeft()
{
	glm::vec2 pos = m_camera.getPosition();
	glm::vec2 newPos(pos.x - 5, pos.y);
	m_camera.setPosition(newPos);
	m_camera.update();
}

void World::moveRight()
{
	glm::vec2 pos = m_camera.getPosition();
	glm::vec2 newPos(pos.x + 5, pos.y);
	m_camera.setPosition(newPos);
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
	//logBodies(m_b2world);
}

void World::render()
{
	
	
	m_b2world->ClearForces();
	m_debugDraw->Clear();

	m_b2world->DrawDebugData();	//

	m_debugDraw->BufferData();	// 

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_colorProgram.use();
	
	glUseProgram(m_colorProgram.getProgramID());
	
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

	// generate agent
	Agent* agent = new Agent(m_GA, m_b2world, ID, x, y);
	m_agents.emplace_back(agent);


}

void World::spawnAgent(GeneticAlgorithm* GA, int ID, int x, int y)	// spawn agent with GA chromes
{
	// generate agents given existing Genetic code
	Agent* agent = new Agent(GA, m_b2world, ID, x, y);
	m_agents.emplace_back(agent);
}

void World::clearAgents(int mode) 
{
	/*
	for (int i = 0; i < m_agents.size(); i++) {
		m_b2world->DestroyBody(m_agents[i]->getAgentBody());
	}*/

	//m_b2world->DestroyBody(m_b2world->GetBodyList());	

	//std::cout << "body list: " << m_b2world->GetBodyList() << std::endl;
	//std::cout << "body count: " << m_b2world->GetBodyCount() << std::endl;

	//std::cout << "agents before: " << m_b2world->GetBodyCount() << std::endl;
	
	for (int i = 0; i < m_b2world->GetJointCount(); i++) {
		m_b2world->DestroyJoint(m_b2world->GetJointList());
		m_b2world->GetJointList()->GetNext();
	}

	//int count = 0;
	while (m_b2world->GetBodyCount() > 2) {

		//for (int i = 0; i < m_b2world->GetBodyCount(); i++) {	// destroy all bodies in the world
		/*
			for (b2Fixture* f = m_b2world->GetBodyList()->GetFixtureList(); f;) {
				b2Fixture* ftd = f;
				f = f->GetNext();
				m_b2world->GetBodyList()->DestroyFixture(ftd);
			}
		*/
			m_b2world->DestroyBody(m_b2world->GetBodyList());
			m_b2world->GetBodyList()->GetNext();
			//std::cout << "count: " << count << std::endl;
			//count++;
		//}
	}
	
	//for (b2Body* b = m_b2world->GetBodyList(); b; /*b = b->GetNext()*/)  // remove GetNext() call
	/*
	{
		if (b->GetUserData() != NULL) {
			//CCSprite *s = (CCSprite *)b->GetUserData();
			//[self removeChild : s cleanup : YES];
			b2Body* next = b->GetNext();  // remember next body before *b gets destroyed
			m_b2world->DestroyBody(b); // do I need to destroy fixture as well(and how?) or it does that for me?
			b = next;  // go to next body
		}
	}*/

	if (mode == 0) {
		m_agents.clear();
		m_agentGenes.clear();
	}
	else if (mode == 1) {

	}


	//std::cout << "agents remaining: " << m_b2world->GetBodyCount() << std::endl;

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
		std::cout << "Position [" << i << "] - Agent:" << m_agents[i]->getGA()->getID() << " Distance x: " << m_agentScores[i] << std::endl;
	}
}

void World::CalculateStatistics()
{
	// calculate mean x distance of entire population - plot of graph
	// calculate Q1 and Q3 of population - plot on graph

	float totalDistance = 0;
	for (int i = 0; i < NUM_AGENT_POP; i++) {
		int distance = m_agentScores[i];
		totalDistance += distance;
	}

	float mean;
	if (totalDistance != 0) 
	{ 
		mean = totalDistance / NUM_AGENT_POP;
	}
	else { 
		mean = 1; 
	}


	std::cout << "EPOCH: " << m_currentEpoch << std::endl;
	std::cout << "BEST AGENT: " << m_agentScores[0] << std::endl;
	std::cout << "WORST AGENT: " << m_agentScores[NUM_AGENT_POP - 1] << std::endl;
	std::cout << "MEAN: " << mean << std::endl;

	int medianIndex = (int) NUM_AGENT_POP / 2;
	std::cout << "MEDIAN: " << m_agentScores[medianIndex] << std::endl;

	// calculate variance
	float varianceTotal = 0;
	for (int i = 0; i < NUM_AGENT_POP; i++) {
		varianceTotal += pow(m_agentScores[i] - mean, 2);
	}
	float variance;
	if (totalDistance != 0)
	{
		variance = varianceTotal / (NUM_AGENT_POP - 1);
	}
	else {
		variance = 1;
	}
	

	std::cout << "VARIANCE: " << variance << std::endl;

	float stdv = sqrt(variance);

	std::cout << "STANDARD DEVIATION: " << stdv << std::endl;

	std::ofstream file2;
	file2.open("BestAgents.txt", std::ofstream::out | std::ios_base::app);
	file2 << m_agents[0]->getGA()->FlattenChrome();
	file2.close();
	m_agents[0]->getGA()->resetStringChrome();

	// save values to file
	std::ofstream file;
	file.open("statData.txt", std::ofstream::out | std::ios_base::app);
	file << m_currentEpoch << "," << m_agentScores[0] << "," << m_agentScores[NUM_AGENT_POP - 1] << "," 
		<< mean << "," << m_agentScores[medianIndex] << "," << variance << "," << stdv << std::endl;
	file.close();

	//std::ofstream file;
	//can't enable exception now because of gcc bug that raises ios_base::failure with useless message
	//file.exceptions(file.exceptions() | std::ios::failbit);



}

void World::saveAgentSore(int pos)
{
	m_agentScores[pos] = m_agents[pos]->getPosition().x;
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
			//if (m_agents[j]->getPosition().x < m_agents[j + 1]->getPosition().x) {
			if (m_agentScores[j] < m_agentScores[j + 1]) {


				//swap(m_agents[j], m_agents[j + 1]);
				Agent* temp = m_agents[j];
				m_agents[j] = m_agents[j + 1];
				m_agents[j + 1] = temp;

				float temp2 = m_agentScores[j];
				m_agentScores[j] = m_agentScores[j + 1];
				m_agentScores[j + 1] = temp2;



			}
		}
	}

	// print sorted list
	for (int i = 0; i < m_agents.size(); i++) {
		std::cout << "x pos: " << m_agentScores[i] << std::endl;
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
	clearAgents(0);	// clear the existing agents from m_agents and m_b2world

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
	generator.seed(time(0));		// set seed of generator
	while (numAgents > NUM_AGENT_POP * CULL_PERCENTAGE) {		// 100 * 0.5 = 50 (cull 50% of populations)	
		//rand = g->generateRandomInt(0, NUM_AGENT_POP);
		//m_agents.erase(m_agents.begin() + rand);

		offset = CullDistribution(generator);
		int randNum = g->generateRandomInt(0, (NUM_AGENT_POP / 10) - 1);
		index = (offset) * (NUM_AGENT_POP / 10) + randNum;
		if (m_fertileAgents[index] != NULL) {
			//std::cout << "index: " << index << " " << "offset: " << offset << "(" << NUM_AGENT_POP << "/" << 10 << ") + " << randNum << std::endl;
			//std::cout << "REMOVED " << m_fertileAgents[index]->getID() << std::endl;
			m_fertileAgents[index] = NULL; // divide by 10 because the size of distrobution
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

	m_agentGenes.clear();	// clear the current generation of agents
	
	SelectAgentPartners();

	CreateSurvivingAgents();				// spawn the agents that survived

}

void World::SelectAgentPartners() 	// takes list of fertile agents selects pairs to breed
{
	// select which agents will mate with each other
	// agents with better fittness scores will be more likely to have offspring

	GeneticAlgorithm* g = new GeneticAlgorithm();
	

	//int sampleSize = (int) NUM_AGEN_POP * CULL_PERCENTAGE;
	//int sampleSize = 50;
	//for (int i = 0; i < NUM_AGENT_POP * CULL_PERCENTAGE; i++) {
	int newAgents = 0;
	generator.seed(time(0));	// set seed of generator
	int num = (int)NUM_AGENT_POP * CULL_PERCENTAGE;
	while (newAgents < num) {
		int map1 = MateSelectionDistrobution(generator);
		int offset = (int)((NUM_AGENT_POP / 10) * CULL_PERCENTAGE);	// calculate offset size
		int index = (map1) * offset + g->generateRandomInt(1, offset);
		int map2 = MateSelectionDistrobution(generator);
		int index2 = (map2) * offset + g->generateRandomInt(0, offset);
		//std::cout << "index1: " << index << std::endl;
		//std::cout << "index2: " << index2 << std::endl;

		if (index != index2) {	// check if the same agent has been choen twice
			
			GeneticAlgorithm* newGenome = nullptr;
			int num = g->generateRandomInt(2, 3);
			switch (num) 
			{	
				case 1: 
					newGenome = BilateralCrossoverAgent(m_fertileAgentsSelection[map1], m_fertileAgentsSelection[map2]);	// bilateral crossover
					break;
				case 2: 
					newGenome = MeanCrossoverAgent(m_fertileAgentsSelection[map1], m_fertileAgentsSelection[map2]); // arithmetic mean crossover
					break;
				case 3: 
					newGenome = UniformCrossoverAgent(m_fertileAgentsSelection[map1], m_fertileAgentsSelection[map2]);	// uniform crossover
					break;	
			}

			//mutate genome
			newGenome->SearchAgentChromes();	// scale muatation
			//MutateAgent(newGenome);				// morphological mutation


			newGenome->setID(++m_agentCount);
			m_agentGenes.push_back(newGenome); // add genome to agent
			newAgents++;
		}
	}

	// displays all fertile agents
	//for (int i = 0; i < (NUM_AGENT_POP * CULL_PERCENTAGE); i++) {	// map culled agents to new array
	//	std::cout << "agent " << i << ": " << m_fertileAgentsSelection[i]->getAgentBody() << std::endl;
	//}

}

void World::MutateAgent(GeneticAlgorithm* newGenome)
{
	newGenome->MutateMorphologyJoin();
}

void World::CreateSurvivingAgents()
{
	for (int i = 0; i < (int)NUM_AGENT_POP * CULL_PERCENTAGE; i++) {
		GeneticAlgorithm* GA = m_fertileAgentsSelection[i]->getGA();
		//spawnAgent(GA, m_fertileAgentsSelection[i]->getID(), 10, 10);
		m_agentGenes.push_back(GA);
	}
}

GeneticAlgorithm* World::UniformCrossoverAgent(Agent* a1, Agent* a2)
{
	std::string a1Chrome = a1->getGA()->FlattenChrome();	// get string of chrome
	std::string a2Chrome = a2->getGA()->FlattenChrome();

	//std::cout << "--------- a1 -------------" << std::endl;
	//std::cout << a1Chrome << std::endl;
	//std::cout << "--------- a2 -------------" << std::endl;
	//std::cout << a2Chrome << std::endl;
	//std::cout << "--------- end -------------" << std::endl;

	GeneticAlgorithm* GA = new GeneticAlgorithm();
	GA->SplitChromes2(a1Chrome, a2Chrome);

	return GA;
}


GeneticAlgorithm* World::MeanCrossoverAgent(Agent* a1, Agent* a2)
{
	std::string a1Chrome = a1->getGA()->FlattenChrome();	// get string of chrome
	std::string a2Chrome = a2->getGA()->FlattenChrome();

	//std::cout << "--------- a1 -------------" << std::endl;
	//std::cout << a1Chrome << std::endl;
	//std::cout << "--------- a2 -------------" << std::endl;
	//std::cout << a2Chrome << std::endl;
	//std::cout << "--------- end -------------" << std::endl;

	GeneticAlgorithm* GA = new GeneticAlgorithm();
	GA->SplitChromes(a1Chrome, a2Chrome);

	return GA;
}

GeneticAlgorithm* World::BilateralCrossoverAgent(Agent* a1, Agent* a2)
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

	return newGenome;		// return new genome



}

void World::EvolveAgents() 
{
	SelectFertileAgents();
}





