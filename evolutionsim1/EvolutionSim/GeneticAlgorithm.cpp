#include "GeneticAlgorithm.h"

#include <random>
#include <ctime>

#include <math.h>
#include <functional>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <stdio.h>
 
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
	// generate wheel chromes
	float min = 0.05f;	// explin why 0.05f
	float minWheelRadius = 0.1f;

	int maxWheels = 1;
	int numWheels = 0;
	LineChromes line;
	LineChromes finalLine;
	//LineChromes line2;

	//generate line chromes
	//auto lines = generateRandomInt(m_symmetrySize, m_symmetrySize);	// generate number of lines of symmetry of agent - default set to 2
	auto lines = generateRandomInt(2, 2);	// generate number of lines of symmetry of agent - default set to 2
	while (lines-- > 0) {

		//m_wheelChromes.speed = generateRandomFloat(0.1f, 1.0f) * 20 + 10; 	// explaim why * 20 + 10
		//m_wheelChromes.radius = generateRandomFloat() + minWheelRadius;	// change to minWheel

		/*
		for (int i = 0; i < generateRandomInt(1, 2); i++)
		{
			for (int i = 0; i < generateRandomInt(1, 2); i++)
			{
				m_lineChromes.angle = generateRandomFloat(2.0f, 3.0f) * 2 * M_PI;
				m_lineChromes.limit = generateRandomFloat(2.0f, 3.0f) * min * M_PI;
				m_lineChromes.width = generateRandomFloat(3.0f, 4.0f) * 0.2f + min;
				m_lineChromes.length = generateRandomFloat(0.5f, 3.0f) * 5 + 1;

				line.addLine(m_lineChromes);
			}
			
			line.angle = generateRandomFloat(2.0f, 3.0f) * 2 * M_PI;
			line.limit = generateRandomFloat(2.0f, 3.0f) * 0.05f * M_PI;
			line.width = generateRandomFloat(3.0f, 4.0f) * 0.2f + 0.05f;
			line.length = generateRandomFloat(0.5f, 3.0f) * 5 + 1;

			finalLine.addLine(line);
		}

		//m_agentChromes.lines.push_back(finalLine);

		//m_lineChromes.clearChromes();
		//line.clearChromes();
		//finalLine.clearChromes();
		*/
		
		m_lineChromes.angle = generateRandomFloat(2.0f, 3.0f) * 2 * M_PI;
		m_lineChromes.limit = generateRandomFloat(2.0f, 3.0f) * min * M_PI;
		m_lineChromes.width = generateRandomFloat(3.0f, 4.0f) * 0.2f + min;
		m_lineChromes.length = generateRandomFloat(0.5f, 3.0f) * 5 + 1;

		// genreate wheel or line
		if (generateRandomBool()) {
			//m_lineChromes.wheel.push_back(m_wheelChromes);
		}
		else {
			//auto lines = generateRandomInt() - 1;	// determins how many line segments there are
			//while (lines-- > 0) {					// while not created all line segments
			m_lineChromes.lines.push_back(m_lineChromes);	// push line segment to line chromes
			
			//if (generateRandomInt(0, 10) < 5) {
			//	m_lineChromes.lines.push_back(m_lineChromes);	// push line segment to line chromes
			//}
			//}
		}

		// create agent chromes -
		m_agentChromes.lines.push_back(m_lineChromes);
		//m_lineChromes.clearChromes();
		

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

void GeneticAlgorithm::OutputChromes(LineChromes chromes, int bodyConnectionNum)
{
	//std::cout << "b" << bodyCount << ":";
	//m_stringChromes += "b" + std::to_string(bodyConnectionNum) + ":";

		/* // No longer used
		if (chromes.wheel.size() > 0) {
			for (int j = 0; j < chromes.wheel.size(); j++) {
				std::cout << "w" << j << ":";
				std::cout << chromes.wheel[j].speed << "," << chromes.wheel[j].radius << ";" << std::endl;
			
			}
		}*/

		if (chromes.lines.size() == 0) {
			//print to console
			//std::cout << "l" << std::endl;
			//std::cout << chromes.angle << "," << chromes.limit << "," << chromes.width << "," << chromes.length << ";" << std::endl;
			//print to memory
			m_stringChromes += "l\n";
			m_stringChromes += std::to_string(chromes.angle) + "," + std::to_string(chromes.limit) + "," + std::to_string(chromes.width) + "," + std::to_string(chromes.length) + ",\n";
		}
		else {
			for (int j = 0; j < chromes.lines.size(); j++) {
				OutputChromes(chromes.lines[j], j);
			}
			// print to console
			//std::cout << "j" << std::endl;
			//std::cout << chromes.angle << "," << chromes.limit << "," << chromes.width << "," << chromes.length << ";" << std::endl;
			// print to memory
			m_stringChromes += "j\n";
			m_stringChromes += std::to_string(chromes.angle) + "," + std::to_string(chromes.limit) + "," + std::to_string(chromes.width) + "," + std::to_string(chromes.length) + ",\n";
		}
}

std::string GeneticAlgorithm::FlattenChrome()
{
	//std::string* agentChrome = nullptr;

	m_stringChromes = ""; // temp

	//std::cout << "\n============Print chromes==========" << std::endl;
	//m_stringChromes += "\n";
	for (int i = 0; i < m_agentChromes.lines.size(); i++) {
		//std::cout << "" << std::endl;
		//std::cout << "b" << i << std::endl;
		//m_stringChromes += "b" + std::to_string(i) + "\n";
		OutputChromes(m_agentChromes.lines[i], 0);
		m_stringChromes += "b\n";
	}

	
	//std::cout << m_stringChromes << std::endl;
	//std::cout << "\n============================" << std::endl;
	//m_stringChromes += "\n============================";

	return m_stringChromes;
}

int convertToken(std::string token) 
{
	// limb joint convertion
	if (token == "l") 
	{
		return 0;
	}
	else if (token == "j") 
	{
		return 1;
	}
}

void GeneticAlgorithm::reconstructAgentChromesomes(std::string chromes)
{
	std::string token;
	std::istringstream stm(chromes);

	//std::cout << "reconstructing after evo:" << std::endl;
	//std::cout << chromes << std::endl;
	//std::cout << "-------------------------" << std::endl;

	//std::cout << "\n========= First tree ============" << std::endl;
	std::getline(stm, token, 'b');
	//std::cout << token << std::endl;
	LineChromes line = reconstructLineChromesomes(token);

	//std::cout << "\n========= Second tree ============" << std::endl;
	std::getline(stm, token, 'b');
	//std::cout << token << std::endl;
	LineChromes line2 = reconstructLineChromesomes(token);
	//std::cout << "\n==========END=============" << std::endl;

	m_agentChromes.clearChromes();	// clear currnet chromes

	m_agentChromes.addLine(line);	// add tree 1 - left side
	m_agentChromes.addLine(line2);	// add tree 2 - right side

	//std::cout << "Reconstricted:" << std::endl;
	//std::cout << "\n========== recon START =============" << std::endl;
	//FlattenChrome();
	//std::cout << "\n========== recon END =============" << std::endl;

}

LineChromes GeneticAlgorithm::reconstructLineChromesomes(std::string chromes)
{
	int letterCount;

	std::string bodyNum;
	std::string limbType;
	
	std::string limbAttribute;

	std::istringstream stm(chromes);	// split string to values
	std::string token;
	std::vector<std::string> chromosomes;

	std::vector<AgentChromes> agentChromes;
	
	std::vector<LineChromes> stack;	// hold the semi constricted chromes

	LineChromes limb;
	LineChromes newLine;

	std::string var;
	int varCount = -1;

	int limbCount = 0;	// keeps track on number of limbs in each vector

	while (std::getline(stm, token, '\n'))
	{
		switch (convertToken(token)) 
		{
			case 0:	// standard limb add - L
				limb.clearChromes();	// clear any values
				std::getline(stm, token, '\n');
				//std::cout << "\n" << std::endl;
				for (int i = 0; i < token.length(); i++) {
					if (token.at(i) != ',') {	
						var += token.at(i);
					} 
					else {
						//std::cout << var << std::endl;
						//i++;	// ignore comma
						varCount++;	// increment attribute type
						switch (varCount) 
						{
							case 0: 
								limb.setAngle(std::atof(var.c_str()));
								break;	// break second switch
							case 1:
								limb.setLimit(std::atof(var.c_str()));
								break; // break second switch
							case 2:
								limb.setWidth(std::atof(var.c_str()));
								break; // break second switch
							case 3:
								limb.setLength(std::atof(var.c_str()));
								varCount = -1;
								break; // break second switch
						}
						var = "";
					}

				}

				stack.push_back(limb);	// push new limb onto stack
				limbCount++;	// increment number of limbs
				//std::cout << limb.toString() << std::endl;


				break;	// break first switch

			case 1: // joint limb add - J
				newLine.clearChromes(); // clear any values
				std::getline(stm, token, '\n');
				//std::cout << "\n" << std::endl;
				for (int i = 0; i < token.length(); i++) {
					if (token.at(i) != ',') {
						var += token.at(i);
					} 
					else {
						//std::cout << var << std::endl;
						//i++;	// ignore comma
						varCount++;	// increment attribute type
						switch (varCount)
						{
						case 0:
							newLine.setAngle(std::atof(var.c_str()));
							break;	// break second switch
						case 1:
							newLine.setLimit(std::atof(var.c_str()));
							break; // break second switch
						case 2:
							newLine.setWidth(std::atof(var.c_str()));
							break; // break second switch
						case 3:
							newLine.setLength(std::atof(var.c_str()));
							varCount = -1;
							break; // break second switch
						}
						var = "";
					}
				}

				
				for (int i = 0; i < limbCount; i++)
				{
					LineChromes line = stack.back();	// get element
					stack.pop_back();	// pop it off stack
					newLine.addLine(line);	// add
				}
				stack.push_back(newLine); // push new line onto stack
				limbCount = stack.size(); // reset limb count to 0
				//std::cout << newLine.toString() << std::endl;	// these are not printing


				break;	// break first switch
		}
	}

	return stack.back();
	
}

std::string GeneticAlgorithm::SplitChromes(std::string a1, std::string a2)
{
	std::string token;
	std::string token2;
	
	std::istringstream stm(a1);	// split string to values
	std::istringstream stm2(a2);	// split string to values

	//std::cout << "\n========= First tree ============" << std::endl;
	std::getline(stm, token, 'b');
	std::getline(stm2, token2, 'b'); 
	std::string newChromesome = CreateCrossOverAgent(token, token2);
	//std::cout << "\n========= First final ============" << std::endl;
	//std::cout << newChromesome << std::endl;

	//std::cout << "\n========= Second tree ============" << std::endl;
	std::getline(stm, token, '\n');	// get rid of newline which both halfs have
	std::getline(stm2, token2, '\n');

	std::getline(stm, token, 'b');
	std::getline(stm2, token2, 'b');

	std::string newChromesome2 = CreateCrossOverAgent(token, token2);
	//std::cout << "\n========= second final ============" << std::endl;
	//std::cout << newChromesome2 << std::endl;

	//combine strings
	std::string fullChromesome = newChromesome + newChromesome2;

	//std::cout << "full chromes start" << std::endl;
	//std::cout << fullChromesome << std::endl;
	//std::cout << "full chromes end" << std::endl;


	reconstructAgentChromesomes(fullChromesome);	// reconstruct the chromes

	return fullChromesome;

}

std::string GeneticAlgorithm::CreateCrossOverAgent(std::string a1, std::string a2)
{
	// split agents by b 
	// add b to string
	// while not empty
		// output straiht to string if instruction
		// get line
		// split up line by ','
		// find the mean of the two values and add to string

	// add b to string
	// repeat for lower half

	std::string newChromesome;

	std::string token;
	std::string token2;

	std::string var;
	std::string var2;
	int varCount = -1;

	float mean = 0;

	std::istringstream stm(a1);	// split string to values
	std::istringstream stm2(a2);	// split string to values

	//std::cout << "a\n";
	//std::cout << a1 << std::endl;
	//std::cout << "a2\n";
	//std::cout << a2 << std::endl;

	//std::getline(stm, token, '\n');
	//std::getline(stm2, token2, '\n');	// get first token of second agent
	while (std::getline(stm, token, '\n'))
	{

		std::getline(stm2, token2, '\n');	// get first token of second agent
		if (token2 != "") {			//check if genome 1 is bigger than genome 2

			// roll 1/2 for which agent structure is used
			// token2 will copy the structure of agent 2
			//if (token == "j" || token == "l") newChromesome += token + "\n";			// GOING WRONG HERE
			newChromesome += token + "\n";
			//std::cout << "1" << std::endl;
			//std::cout << "token: " << token << std::endl;
			//std::cout << "token2: " << token2 << std::endl;

			std::getline(stm, token, '\n');
			std::getline(stm2, token2, '\n');
			//std::cout << "2" << std::endl;
			//std::cout << "token: " << token << std::endl;
			//std::cout << "token2: " << token2 << std::endl;


			//std::cout << "\n" << std::endl;
			for (int i = 0; i < token.length(); i++) {
				if (token.at(i) != ',') {
					var += token.at(i);
					var2 += token2.at(i);
				}
				else {
					//std::cout << var << std::endl;
					//i++;	// ignore comma
					varCount++;	// increment attribute type
					switch (varCount)
					{
					case 0:
						mean = (std::atof(var.c_str()) + std::atof(var2.c_str())) / 2;
						newChromesome += std::to_string(mean) + ",";
						break;	// break second switch
					case 1:
						mean = (std::atof(var.c_str()) + std::atof(var2.c_str())) / 2;
						newChromesome += std::to_string(mean) + ",";
						break; // break second switch
					case 2:
						mean = (std::atof(var.c_str()) + std::atof(var2.c_str())) / 2;
						newChromesome += std::to_string(mean) + ",";
						break; // break second switch
					case 3:
						mean = (std::atof(var.c_str()) + std::atof(var2.c_str())) / 2;
						newChromesome += std::to_string(mean) + ",\n";
						varCount = -1;
						break; // break second switch
					}
					var = "";
					var2 = "";
				}
			}
		}
	}
	newChromesome += "b\n";
	return newChromesome;
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

