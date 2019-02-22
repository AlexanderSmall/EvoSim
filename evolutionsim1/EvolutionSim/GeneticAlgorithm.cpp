#include "GeneticAlgorithm.h"

#include <random>
#include <ctime>

#include <math.h>
#include <functional>
#include <iostream>

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

int GeneticAlgorithm::get_rand() {
	return 1;
}

float GeneticAlgorithm::generateRandomFloat()
{
	//std::mt19937 generator{ 37 };
	std::uniform_real_distribution<float> dfloat{ m_minFloatLimit, m_maxFloatLimit };
	//std::cout << "RF: " << dfloat(m_RandEng) << std::endl;
	return dfloat(m_RandEng);
}

float GeneticAlgorithm::generateRandomInt()
{
	//std::mt19937 generator{ 37 };
	std::uniform_int_distribution<int> dint{ m_minIntLimit, m_maxIntLimit };
	//std::cout << "RI: " << dint(m_RandEng) << std::endl;
	return dint(m_RandEng);
}

bool GeneticAlgorithm::generateRandomBool()
{
	//std::mt19937 generator{ 37 };
	std::uniform_int_distribution<int> dbool{0, 1};
	return dbool(m_RandEng);
}


void GeneticAlgorithm::generateChromosomes()
{
	// generate wheel chromes
	float min = 0.05f;	// explin why 0.05f
	m_wheelChromes.speed = generateRandomFloat() * 20  + 10; 	// explaim why * 20 + 10
	m_wheelChromes.radius = generateRandomFloat() + min;

	//generate line chromes

	m_lineChromes.limit = generateRandomFloat() * min * M_PI;
	m_lineChromes.width = generateRandomFloat() * 0.2f + min;
	m_lineChromes.length = generateRandomFloat() * 5 + 1;

	std::cout << "lineChromes.angle: " << m_lineChromes.angle << std::endl;

	if (generateRandomBool()) {
		m_lineChromes.wheel.push_back(m_wheelChromes);
	}
	else {
		auto lines = generateRandomInt() - 1;
		while (lines-- > 0) {
			m_lineChromes.lines.push_back(m_lineChromes);
		}
	}

	// create agent chromes
	auto lines = generateRandomInt() - 1;
	std::cout << "rand int: " << lines << std::endl;
	while (lines-- > 0) {
		m_agentChromes.lines.push_back(m_lineChromes);
	}

}	


// seperate chromosomes 
/*
void GeneticAlgorithm::generateMagnitudesAndAngles()
{
	
	std::cout << "1.1" << std::endl;
	for (int i = 0; i < NUM_AGENT_POP; i++) {
		float angles[MAX_AGENT_BODY_EDGES];
		float angleSum = 0;
		for (int j = 0; j < MAX_AGENT_BODY_EDGES; j++) {
			std::cout << *m_chromosomes[i][j] << std::endl;
			*m_magnitudes[i][j] = *m_chromosomes[i][j];
			angles[j] = *m_chromosomes[i][j + 8];
			angleSum += angles[j];	
		}

		// sum of angles must equal (n - 2)*180

		float angle = 0;
		for (int j = 0; j < MAX_AGENT_BODY_EDGES; j++) {	
			std::cout << "1.3" << std::endl;
			*m_angles[i][j] = (angle + angles[j] / angleSum * M_PI * 2);
			angle = *m_angles[i][j];
		}
	}
	std::cout << "1.2" << std::endl;
}
*/


void GeneticAlgorithm::resetDefinitions()
{
	m_agentChromes.lines.clear();
	//m_wheelChromes;
	m_lineChromes.lines.clear();
	m_lineChromes.wheel.clear();
}


/* ------ spare code
// 0 - 7 (megnitude chromosomes)
int magnitudeSize = 8;
int magnitudeOffSet = 0;
// 8 - 15 (angle chromosomes)
int angleSize = 8;
int angleOffSet = 8;
// 16 - 17 (wheel position chromosomes)

//static std::mt19937 randEng;
//randEng.seed(time(nullptr));
static std::default_random_engine randEng;
static std::uniform_real_distribution<> randMagnitude(0.2f, 1.0f);	// magnitude range
static std::uniform_real_distribution<> randAngle(0.2f, 1.0f);	// angle range


for (int i = 0; i < NUM_AGENT_POP; i++) {
	// magnitudes
	for (int j = 0; j < magnitudeSize; j++) {
		float num;
		num = randMagnitude(randEng);
		m_chromosomes[i][j] = &num; // store value in chromosomes
		m_magnitudes[i][j] = &num;	// store magnitude chromosomes in array of magnitudes
		//std::cout << *m_chromosomes[i][j] << std::endl;
	}

	// angles
	for (int j = 0; j < angleSize; j++) {
		float num;
		num = randAngle(randEng);
		m_chromosomes[i][j + angleOffSet] = &num;
		m_angles[i][j] = &num;
	}

	//wheels

}

// Make angles sum to the interior of polygon
*/