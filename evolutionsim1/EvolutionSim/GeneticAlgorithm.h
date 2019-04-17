#pragma once

#include <random>
#include <ctime>
#include <stdio.h>
#include <stdio.h>
#include <string>

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

	void setAngle(float angle) { this->angle = angle; };
	void setLimit(float limit) { this->limit = limit; };
	void setWidth(float width) { this->width = width; };
	void setLength(float length) { this->length = length; };

	void addLine(LineChromes line) { lines.emplace_back(line); };

	std::string toString() { return "a: " + std::to_string(this->angle)+ " li: " +
		std::to_string(this->limit) + " w: " + std::to_string(this->width) + " le: " + std::to_string(this->length); };

	void clearChromes() {
		lines.clear();
		this->angle = 0;
		this->limit = 0;
		this->width = 0;
		this->length = 0;
	}
};

struct AgentChromes {
	std::vector<LineChromes> lines;

	void addLine(LineChromes line) {
		lines.emplace_back(line);
	}

	void clearChromes() {
		lines.clear();
	}
};

class GeneticAlgorithm
{
public:
	GeneticAlgorithm();
	~GeneticAlgorithm();

	float generateRandomFloat();
	float generateRandomInt();
	bool generateRandomBool();

	float generateRandomInt(int i1, int i2);
	float generateRandomFloat(float f1, float f2);

	void generateChromosomes();
	
	// getters
		// Agent chromes
	AgentChromes getAgentChromes() { return m_agentChromes; }

	//wheel chromes
	WheelChromes getWheelChromes() { return m_wheelChromes; }

	WheelChromes getWheelChromes(int lineIndex, int wheelIndex);

	// line (body) chromes
	LineChromes getLineChromes() { return m_lineChromes; }

	void outputAgentChromes();


	void MutateMorphologyJoin();
	bool MutateMorphology(LineChromes line, int path);
	void MutateGenome(LineChromes chromes, int bodyCount);
	void SearchAgentChromes();

	void addAgentChromes(LineChromes line);

	LineChromes reconstructLineChromesomes(std::string chromes);
	void reconstructAgentChromesomes(std::string chromes);

	void resetDefinitions();

	void OutputChromes(LineChromes chromes, int bodyConnectionNum);

	std::string FlattenChrome();


	std::string SplitChromes(std::string a1, std::string a2);
	std::string CreateCrossOverAgent(std::string a1, std::string a2);
	void resetStringChrome();

	int getSymmetrySize() { return m_symmetrySize; }
	void setSymmetrySize(int symm) { m_symmetrySize = symm; }

	int getID() { return ID; };
	void setID(int id) { ID = id; }

private:

	int ID = -1;

	int RANDOM_ENGINE_SEED;

	int m_symmetrySize = 2;

	float m_mutationRate = 0.4;
	float m_mutationChance = 0.05;

	// REMOVE:
	//limits - may remove access from GA to range limits of random variables
	float m_minFloatLimit = 2.0f, m_maxFloatLimit = 3.0f;
	int m_minIntLimit = 1, m_maxIntLimit = 3;

	std::string m_stringChromes;

	// Agent chromes
	AgentChromes m_agentChromes;

	//wheel chromes
	WheelChromes m_wheelChromes;

	// line (body) chromes
	LineChromes m_lineChromes;

	bool remove = false;

	// NOTE :: CREATE GNOME CONVENIENT FOR BREEDING/EVOLUTION
	// convert Agent chromes to linear vector - how to convert back into agent? maybe not
	// traverse Agent chromes - use recurrsion 



};

