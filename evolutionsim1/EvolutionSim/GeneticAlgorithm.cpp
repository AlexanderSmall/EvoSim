#include "GeneticAlgorithm.h"

#include <random>
#include <ctime>

#include <math.h>
#include <functional>
#include <iostream>
#include <string>
 
#define M_PI acos(-1.0)

static std::random_device rd; // random device engine, usually based on /dev/random on UNIX-like systems
// initialize Mersennes' twister using rd to generate the seed
std::mt19937 m_RandEng(rd());

GeneticAlgorithm::GeneticAlgorithm()
{
	//RANDOM_ENGINE_SEED = get_rand(1, 100);
	static std::uniform_int_distribution<int> uid(1, 6); // random dice
	uid(m_RandEng); // use rng as a generator
}


GeneticAlgorithm::~GeneticAlgorithm()
{
}

float GeneticAlgorithm::generateRandomFloat()
{
	std::uniform_real_distribution<float> dfloat{ m_minFloatLimit, m_maxFloatLimit };
	//std::cout << "RF: " << dfloat(m_RandEng) << std::endl;
	return dfloat(m_RandEng);
}

float GeneticAlgorithm::generateRandomInt()
{
	std::uniform_int_distribution<int> dint{ m_minIntLimit, m_maxIntLimit };
	//std::cout << "RI: " << dint(m_RandEng) << std::endl;
	return dint(m_RandEng);
}

bool GeneticAlgorithm::generateRandomBool()
{
	std::uniform_int_distribution<int> dbool{0, 1};
	return dbool(m_RandEng);
}

float GeneticAlgorithm::generateRandomFloat(float f1, float f2)
{
	std::uniform_real_distribution<float> dfloat{ f1, f2 };
	float RF = dfloat(m_RandEng);
	return RF;
}

float GeneticAlgorithm::generateRandomInt(int i1, int i2)
{
	std::uniform_int_distribution<int> dint{ i1, i2 };
	int RI = dint(m_RandEng);
	return RI;
}

WheelChromes GeneticAlgorithm::getWheelChromes(int lineIndex, int wheelIndex)
{ 
	return m_agentChromes.lines[lineIndex].wheel[wheelIndex];
}


void GeneticAlgorithm::generateChromosomes()
{
	/*
	// generate wheel chromes
	float min = 0.05f;	// explin why 0.05f
	float minWheelRadius = 0.2f;


	m_wheelChromes.speed = generateRandomFloat() * 20  + 10; 	// explaim why * 20 + 10
	m_wheelChromes.radius = generateRandomFloat() + minWheelRadius;	// change to minWheel

	//generate line chromes

	m_lineChromes.angle = generateRandomFloat() * 2 * M_PI;
	m_lineChromes.limit = generateRandomFloat() * min * M_PI;
	m_lineChromes.width = generateRandomFloat() * 0.2f + min;
	m_lineChromes.length = generateRandomFloat() * 5 + 1;


	// genreate wheel or line
	if (generateRandomBool()) {
		m_lineChromes.wheel.push_back(m_wheelChromes);
	}
	else {
		auto lines = generateRandomInt() - 1;	// determins how many line segments there are
		while (lines-- > 0) {					// while not created all line segments
			m_lineChromes.lines.push_back(m_lineChromes);	// push line segment to line chromes
		}
	}
	

	// create agent chromes - 
	auto lines = generateRandomInt() - 1;
	while (lines-- > 0) {
		m_agentChromes.lines.push_back(m_lineChromes);
	}
	*/

	// generate wheel chromes
	float min = 0.05f;	// explin why 0.05f
	float minWheelRadius = 0.1f;

	int maxWheels = 1;
	int numWheels = 0;

	//generate line chromes

	auto lines = generateRandomInt(m_symmetrySize, m_symmetrySize);
	while (lines-- > 0) {

		m_wheelChromes.speed = generateRandomFloat(0.1f, 1.0f) * 20 + 10; 	// explaim why * 20 + 10
		m_wheelChromes.radius = generateRandomFloat() + minWheelRadius;	// change to minWheel

		m_lineChromes.angle = generateRandomFloat(2.0f, 3.0f) * 2 * M_PI;
		m_lineChromes.limit = generateRandomFloat(2.0f, 3.0f) * min * M_PI;
		m_lineChromes.width = generateRandomFloat(3.0f, 4.0f) * 0.2f + min;
		m_lineChromes.length = generateRandomFloat(0.5f, 4.0f) * 5 + 1;

		// genreate wheel or line
		if (generateRandomBool()) {
			//m_lineChromes.wheel.push_back(m_wheelChromes);
		} 
		else {
			//auto lines = generateRandomInt() - 1;	// determins how many line segments there are
			//while (lines-- > 0) {					// while not created all line segments
			m_lineChromes.lines.push_back(m_lineChromes);	// push line segment to line chromes
			//}
		}

		// create agent chromes - 
		m_agentChromes.lines.push_back(m_lineChromes);
	}
	
}	


void GeneticAlgorithm::resetDefinitions()
{
	m_agentChromes.lines.clear();
	//m_wheelChromes;
	m_lineChromes.lines.clear();
	m_lineChromes.wheel.clear();
}

void GeneticAlgorithm::addAgentChromes(LineChromes line)
{
	m_agentChromes.lines.emplace_back(line);
}

void GeneticAlgorithm::OutputChromes(LineChromes chromes, int bodyCount)
{
	//std::cout << "b" << bodyCount << ":";
	m_stringChromes += "b" + std::to_string(bodyCount) + ":";
		//std::cout << chromes.angle << "," << chromes.limit << "," << chromes.width << "," << chromes.length << ";" << std::endl;
		m_stringChromes += std::to_string(chromes.angle) + "," + std::to_string(chromes.limit), "," + std::to_string(chromes.width) + "," + std::to_string(chromes.length) + ";" + "\n";
		m_stringChromes += "\n";

		if (chromes.wheel.size() > 0) {
			for (int j = 0; j < chromes.wheel.size(); j++) {
				std::cout << "w" << j << ":";
				std::cout << chromes.wheel[j].speed << "," << chromes.wheel[j].radius << ";" << std::endl;
			
			}
		}

		if (chromes.lines.size() > 0) {
			for (int j = 0; j < chromes.lines.size(); j++) {
				OutputChromes(chromes.lines[j], j);
			}
		}
}

std::string GeneticAlgorithm::FlattenChrome()
{
	std::string* agentChrome = nullptr;

	for (int i = 0; i < m_agentChromes.lines.size(); i++) {
		//std::cout << "" << std::endl;
		//std::cout << "L" << i << ":" << std::endl;
		m_stringChromes += "\nL" + std::to_string(i) + ":" + "\n";
		OutputChromes(m_agentChromes.lines[i], 0);
	}

	std::cout << m_stringChromes << std::endl;
	m_stringChromes += "\n============================";
	return m_stringChromes;
}

void GeneticAlgorithm::resetStringChrome()
{
	m_stringChromes = "";
}

void GeneticAlgorithm::MutateGenome(LineChromes chromes, int bodyCount)
{
	//std::cout << "b" << bodyCount << ":";
	//std::cout << chromes.angle << "," << chromes.limit << "," << chromes.width << "," << chromes.length << ";" << std::endl;

	GeneticAlgorithm* g = new GeneticAlgorithm();

	//angle 
	if (g->generateRandomFloat(0.0f, 1.0f) <= m_mutationChance) {
		if (g->generateRandomBool()) {
			chromes.setAngle(chromes.angle + (chromes.angle * m_mutationRate));
		}
		else {
			chromes.setAngle(chromes.angle - (chromes.angle * m_mutationRate));
		}
		//std::cout << "angle change" << std::endl;
	}
	// limit
	if (g->generateRandomFloat(0.0f, 1.0f) <= m_mutationChance) {
		if (g->generateRandomBool()) {
			chromes.setLimit(chromes.limit + (chromes.limit * m_mutationRate));
		}
		else {
			chromes.setLimit(chromes.limit - (chromes.limit * m_mutationRate));
		}
		//std::cout << "limit change" << std::endl;
	}
	// width
	if (g->generateRandomFloat(0.0f, 1.0f) <= m_mutationChance) {
		if (g->generateRandomBool()) {
			chromes.setWidth(chromes.width + (chromes.width * m_mutationRate));
		}
		else {
			chromes.setWidth(chromes.width - (chromes.width * m_mutationRate));
		}
		//std::cout << "width change" << std::endl;
	}
	// length
	if (g->generateRandomFloat(0.0f, 1.0f) <= m_mutationChance) {
		if (g->generateRandomBool()) {
			chromes.setLength(chromes.length + (chromes.length * m_mutationRate));
		}
		else {
			chromes.setLength(chromes.length - (chromes.length * m_mutationRate));
		}
		//std::cout << "length change" << std::endl;
	}

	if (chromes.lines.size() > 0) {
		for (int j = 0; j < chromes.lines.size(); j++) {
			MutateGenome(chromes.lines[j], j);
		}
	}

}

void GeneticAlgorithm::SearchAgentChromes()
{
	for (int i = 0; i < m_agentChromes.lines.size(); i++) {
		MutateGenome(m_agentChromes.lines[i], 0);
	}
}

void GeneticAlgorithm::outputAgentChromes()
{
	std::cout << "_______________________" << std::endl;
	std::cout << "--NEW GENOME CREATED--" << std::endl;
	std::cout << "_______________________" << std::endl;
	for (int i = 0; i < m_agentChromes.lines.size(); i++) {
		std::cout << "Main body angle: " << m_agentChromes.lines[i].angle << std::endl;
		std::cout << "Main body limit: " << m_agentChromes.lines[i].limit << std::endl;
		std::cout << "Main body width: " << m_agentChromes.lines[i].width << std::endl;
		std::cout << "Main body length: " << m_agentChromes.lines[i].length << std::endl;

		std::cout << " LINE ATTACHENTS:" << std::endl;
		std::cout << "____________________________________________" << std::endl;
		for (int j = 0; j < m_agentChromes.lines[i].lines.size(); j++) {
			std::cout << "Line attachment " << j << std::endl;
			std::cout << "     attachment angle: " << m_agentChromes.lines[i].lines[j].angle << std::endl;
			std::cout << "     attachment limit: " << m_agentChromes.lines[i].lines[j].limit << std::endl;
			std::cout << "     attachment width: " << m_agentChromes.lines[i].lines[j].width << std::endl;
			std::cout << "     attachment length: " << m_agentChromes.lines[i].lines[j].length << std::endl;
		}

		std::cout << "____________________________________________" << std::endl;
		for (int j = 0; j < m_agentChromes.lines[i].wheel.size(); j++) {
			std::cout << "Wheel attachment " << j << std::endl;
			std::cout << "     wheel speed: " << m_agentChromes.lines[i].wheel[j].speed << std::endl;
			std::cout << "     wheel radius: " << m_agentChromes.lines[i].wheel[j].radius << std::endl;
		}
	}
}

