#pragma once
#include "World.h"

#include <iostream>
#include <Box2D/Box2D.h>

constexpr char const* const pPositionAttribName = "position";
constexpr char const* const pColourAttribName = "colour";

const int GRAVITY = -9.81;

// create initialisation lists

World::World(odingine::Window* window) : m_mvpAttribLoc(0), m_pMvpMatStart(0)
{
	m_b2world = nullptr;

	m_window = window;
		// INIT SHADERS
	initShaders();
	initWorld();
	//sortAgentParts();
}

void World::initWorld()
{
	// Set up scene for rendering.
	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);

	// Init Renderer
	
	// DEBUG DRAW DOES NOT CONTAIN VERTICES IF INITIALISED LIKE THIS
	// 
	//b2draw::DebugDraw test{ glGetAttribLocation(m_colorProgram.getProgramID(), pPositionAttribName),
	//							glGetAttribLocation(m_colorProgram.getProgramID(), pColourAttribName), 16, 0.f };

	m_debugDraw = new b2draw::DebugDraw(glGetAttribLocation(m_colorProgram.getProgramID(), pPositionAttribName),
		glGetAttribLocation(m_colorProgram.getProgramID(), pColourAttribName), 16, 0.f);



	//b2draw::DebugDraw m_debugDraw{glGetAttribLocation(m_colorProgram.getProgramID(), pPositionAttribName),
	//						   	glGetAttribLocation(m_colorProgram.getProgramID(), pColourAttribName), 16, 0.f};


	
	//m_debugDraw = &test;

	m_debugDraw->SetFlags(0xff);
	std::cout << m_debugDraw->GetFlags() << " - flags" << std::endl;

	createWorld();

	initCamera();

	// create agents
	//createAgents();
	spawnAgent(7, 7);


	//createAgentsTwo(); // UNCOMMENT
	//createAgentsTwo();


	//m_b2world->SetDebugDraw(m_renderer);
	//m_renderer->SetFlags(b2Draw::e_shapeBit + b2Draw::e_jointBit); // can't set flags for some reason
}

void World::createAgentsTwo() 
{
	b2Filter collideEverything;
	collideEverything.categoryBits = 0x0001;
	collideEverything.maskBits = 0xffff;
	collideEverything.groupIndex = 1;

	b2FixtureDef fixtureDef;
	fixtureDef.density = 1.0f;
	//fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.filter = collideEverything;

	
		b2BodyDef bodyDef1;
		bodyDef1.type = b2_staticBody;
		bodyDef1.position.Set(0.0f, -4.0f);
		auto const pGroundBody = m_b2world->CreateBody(&bodyDef1);

		b2PolygonShape box;
		box.SetAsBox(-30.0f, 1.0f);

		fixtureDef.shape = &box;
		pGroundBody->CreateFixture(&fixtureDef);
	

	
		b2BodyDef bodyDef2;
		bodyDef2.type = b2_dynamicBody;
		bodyDef2.position.Set(0.0f, 4.0f);

		// DEBUG DOESN'T STEP INTO CreateBody() may reason things are being printed because not configured?
		// 

		auto const pDynamicBody = m_b2world->CreateBody(&bodyDef2);

		b2PolygonShape box2;
		box2.SetAsBox(1.0f, 1.0f);

		fixtureDef.shape = &box2;
		pDynamicBody->CreateFixture(&fixtureDef);
	

	
		b2BodyDef bodyDef3;
		bodyDef3.type = b2_dynamicBody;
		bodyDef3.position.Set(-7.0f, 8.0f);
		bodyDef3.linearVelocity.Set(1.0f, 0.0f);
		bodyDef3.angularVelocity = 1.5f;
		auto const pBody = m_b2world->CreateBody(&bodyDef3);

		b2CircleShape circle;
		circle.m_p = { 0.0f, 0.0f };
		circle.m_radius = 2.0f;

		fixtureDef.shape = &circle;
		pBody->CreateFixture(&fixtureDef);
	
}

void World::initCamera()
{
	
	// Init camera
	m_camera.init(m_screenWidth, m_screenHeight);
	m_camera.setScale(10.0f);
	// test for camera
	m_camera.setPosition(glm::vec2(0, 0));
	
	/*
	constexpr float fieldOfView{ 45.0f };
	auto const projMat = glm::perspective(
		fieldOfView,
		4.0f / 3.0f,
		0.1f,
		100.0f
	);

	const glm::vec3 eye{ 0.0f, 0.0f, 40.0f };	// may need to change back to constexpr
	const glm::vec3 focus{ 0.0f, 0.0f, 0.0f };
	const glm::vec3 up{ 0.0f, 1.0f, 0.0f };
	auto const viewMat = glm::lookAt(eye, focus, up);

	glm::mat4 const modelMat{ 1.0f };

	auto mvpMat{ projMat * viewMat * modelMat };
	auto pMvpMatStart{ &mvpMat[0][0] };
	m_pMvpMatStart = pMvpMatStart;				// something wrong round here?

	auto mvpAttribLoc{ glGetUniformLocation(m_colorProgram.getProgramID(), "MVP") };
	m_mvpAttribLoc = mvpAttribLoc;

	if (m_mvpAttribLoc < 0)
	{
		throw std::runtime_error{ "Unable to locate uniform 'MVP'" };
	}

	std::cout << m_pMvpMatStart << std::endl;
	std::cout << m_mvpAttribLoc << std::endl;
	*/
}

void World::createWorld() {



	// Create the world
	b2Vec2 const gravity(0.0f, GRAVITY);
	m_b2world = new b2World(gravity);

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -20.0f);

	b2Body* groundBody = m_b2world->CreateBody(&groundBodyDef);

	b2PolygonShape groundBox;
	groundBox.SetAsBox(150.0f, 2.0f);

	groundBody->CreateFixture(&groundBox, 0.0f);

	/*
	b2Filter collideEverything;
	collideEverything.categoryBits = 0x0001;
	collideEverything.maskBits = 0xffff;
	collideEverything.groupIndex = 1;

	b2FixtureDef fixtureDef;
	fixtureDef.density = 1.0f;
	//fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.filter = collideEverything;
	*/




	// Set up debug draw
	m_b2world->SetDebugDraw(m_debugDraw);	// init debugdraw

	//m_b2world->SetContinuousPhysics(true);
	//m_b2world->SetAutoClearForces(true);
	/*
	b2Filter collideEverything;
	collideEverything.categoryBits = 0x0001;
	collideEverything.maskBits = 0xffff;
	collideEverything.groupIndex = 1;

	b2FixtureDef fixtureDef;
	fixtureDef.density = 1.0f;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.filter = collideEverything;

	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(0.0f, -4.0f);
	auto const pGroundBody = m_b2world->CreateBody(&bodyDef);

	b2PolygonShape box;
	box.SetAsBox(-30.0f, 1.0f);

	fixtureDef.shape = &box;
	pGroundBody->CreateFixture(&fixtureDef);
	*/
}

void World::createAgents()
{
	int x = 0;
	int y = 0;

	for (int i = 0; i < 1; i++) {

		// generate Genetic material for agent
		m_GA = new GeneticAlgorithm();
		m_GA->generateChromosomes();

		// generate agents
		agent = new Agent(m_GA, m_b2world, x, y);
		// m_agents.emplace_back(m_b2world); // here is the error
		m_agents.emplace_back(agent);
	}
}

void World::updateWorld()
{
	//m_camera.update();	// maybe remove

	//check input here


	// Update the physics simulation
	//m_b2world->Step(1.0f / 60.0f, 6, 2);
	//m_b2world->DrawDebugData();

}

void World::setCameraPosition(glm::vec2 position)
{
	m_camera.setPosition(position); // set camera position
	m_camera.update();
}

void World::initShaders() {
	// Compile our color shader
	m_colorProgram.compileShaders("Shaders/vertexShader.vs", "Shaders/fragmentShader.fs");
	//m_colorProgram.addAttribute(pPositionAttribName);
	//m_colorProgram.addAttribute(pColourAttribName);
	//m_colorProgram.addAttribute("vertexUV");
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

void World::compute()
{
	//b2draw::DebugDraw m_debugDraw
	//b2draw::DebugDraw debug{glGetAttribLocation(m_colorProgram.getProgramID(), pPositionAttribName),
	//	glGetAttribLocation(m_colorProgram.getProgramID(), pColourAttribName), 16, 0.f };
	//debug = m_debugDraw
	//b2draw::DebugDraw debugDraw{ glGetAttribLocation(m_colorProgram.getProgramID(), pPositionAttribName),
	//	glGetAttribLocation(m_colorProgram.getProgramID(), pColourAttribName), 16, 0.f };
	

	/*
	b2World* b2world;
	b2world = m_b2world;

	auto const update = [&debugDraw, &b2world] {
		b2world->Step(1.0f / 60.0f, 6, 2);
		b2world->ClearForces();
		debugDraw.Clear();
		//m_debugDraw.SetFlags(m_debugDraw.e_shapeBit | m_debugDraw.e_jointBit);

		// follow agent
		//int x = m_agents[0]->getPosition().x;
		//int y = m_agents[0]->getPosition().y;
		//m_camera.setPosition(glm::vec2(x, y));

		b2world->DrawDebugData();	//
		debugDraw.BufferData();
		logBodies(b2world);
	};

	GLuint programID;
	programID = m_colorProgram.getProgramID();


	SDL_Window* pSDLWindow = m_window->getPWindow();
	

	auto const render = [&debugDraw, programID, pSDLWindow] {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);
		//glUniformMatrix4fv(mvpAttribLoc, 1, GL_FALSE, pMvpMatStart);
		debugDraw.Render();
		SDL_GL_SwapWindow(pSDLWindow);
	};
	
	while (1 == 1) {
		update();
		render();
	}
	*/

}

void World::update()
{
	//m_camera.update();	// maybe remove
	m_b2world->Step(1.0f / 60.0f, 6, 2);
	m_b2world->ClearForces();
	m_debugDraw->Clear();
	//m_debugDraw.SetFlags(m_debugDraw.e_shapeBit | m_debugDraw.e_jointBit);

	// follow agent
	int x = m_agents[0]->getPosition().x;
	int y = m_agents[0]->getPosition().y;
	m_camera.setPosition(glm::vec2(x, y));
	m_camera.update();

	// CAN NOT RETRIEVE FLAG SET FOR DebugDraw
	m_b2world->DrawDebugData();	//


	// DATA NOT BEING SENT TO BUFFE???

	m_debugDraw->BufferData();	// data is not being buffered????!!
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

	//SDL_GL_SwapWindow(m_window->getPWindow());
}

void World::drawGame() {		// NOT USED
}

void World::spawnAgent(int x, int y) 
{ 
	if (m_agents.size() > 0) {
		for (int i = 0; i < m_agents.size(); i++) {
			//m_b2world->GetBodyList().
		}
	}
	//m_clickX = x; 
	//m_clickY = y; 
	std::cout << "NEW AGENT" << std::endl;
	// generate Genetic material for agent
	m_GA = new GeneticAlgorithm();
	m_GA->generateChromosomes();

	// generate agents
	agent = new Agent(m_GA, m_b2world, x, y);
	//agent->setPosition(m_clickX, m_clickY);
	// m_agents.emplace_back(m_b2world); // here is the error
	m_agents.emplace_back(agent);
}


void World::sortAgentParts()
{
	for (int i = 0; i < m_agents.size(); i++) {

		std::cout << "agent fixture list: " << m_agents[i]->getAgentBody()->GetFixtureList() << std::endl;

		std::cout << "agent position: " << m_agents[i]->getAgentBody()->GetPosition().x << ", " << m_agents[i]->getAgentBody()->GetPosition().y << std::endl;

		std::vector<b2Body*> parts;
		parts = m_agents[i]->getAgentParts();

		for (int j = 0; j < parts.size(); j++) {
			std::cout << "type: " << parts[j] << std::endl;
		
		}

		std::cout << "-------------------" << std::endl;

	}

}


