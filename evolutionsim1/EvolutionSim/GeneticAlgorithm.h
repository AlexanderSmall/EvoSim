#pragma once

#include <random>
#include <ctime>


static const int NUM_AGENT_POP = 10;
static const int MAX_AGENT_BODY_EDGES = 8;
static const int NUM_CHROMES = 16;
static const float MIN_CART = 0.1;
static const float MAX_CART = 3;
static const float MIN_WHEEL = 0.1;
static const float MAX_WHEEL = 1.5;
static const float MIN_ANGLE = 0.08;


struct WheelChromes {
	float speed;
	float radius;
};

struct LineChromes {
	float angle;
	float limit;
	float width;
	float length;

	std::vector<WheelChromes> wheel;
	std::vector<LineChromes> lines;
};

struct AgentChromes {
	std::vector<LineChromes> lines;
};

class GeneticAlgorithm
{
public:
	GeneticAlgorithm();
	~GeneticAlgorithm();

	int get_rand();

	float generateRandomFloat();
	float generateRandomInt();
	bool generateRandomBool();

	void generateChromosomes();

	// getters
		// Agent chromes
	AgentChromes getAgentChromes() { return m_agentChromes; }

	//wheel chromes
	WheelChromes getWheelChromes() { return m_wheelChromes; }

	// line (body) chromes
	LineChromes getLineChromes() { return m_lineChromes; }

	void resetDefinitions();



	//void generateMagnitudesAndAngles();

	//float getMagnitudes(int agentIndex, int lineIndex) { return *m_magnitudes[agentIndex][lineIndex];	 }
	//float getAngles(int agentIndex, int lineIndex) { return *m_angles[agentIndex][lineIndex]; }

	

private:

	//static const int RANDOM_ENGINE_SEED = 1;
	//int RANDOM_ENGINE_SEED = 1;
	int RANDOM_ENGINE_SEED;

	//limits - may remove access from GA to range limits of random variables
	float m_minFloatLimit = 1.0f, m_maxFloatLimit = 3.0f;
	int m_minIntLimit = 1, m_maxIntLimit = 2;


	// Agent chromes
	AgentChromes m_agentChromes;

	//wheel chromes
	WheelChromes m_wheelChromes;

	// line (body) chromes
	LineChromes m_lineChromes;

	// 

	//float* m_magnitudes[NUM_AGENT_POP][MAX_AGENT_BODY_EDGES];	// buffer holds magnitude info for each agent
	//float* m_angles[NUM_AGENT_POP][MAX_AGENT_BODY_EDGES];

	//float* m_chromosomes[NUM_AGENT_POP][NUM_CHROMES];



};

