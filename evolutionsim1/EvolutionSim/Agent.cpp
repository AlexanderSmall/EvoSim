#pragma once

#include "Agent.h"

#include <math.h>

#include <iostream>

struct AgentBuilder {

	b2World& m_b2world;

	GeneticAlgorithm m_GA;

	std::vector<b2Body*> m_agentParts;

	// constants
	b2CircleShape m_wheelShape;	// initalise m_wheelShape
	b2FixtureDef m_wheelFixture;	// initalise m_wheelFixture
	b2BodyDef m_wheelBody;		// initalise m_wheelBody

	b2PolygonShape m_lineShape;	// initialise Agent body lines	
	b2FixtureDef m_lineFixture;	// initialise line fixture
	b2BodyDef m_lineBody;			// 	initalise body definition

	b2PolygonShape m_boxShape;	// 
	b2FixtureDef m_boxFixture;
	b2BodyDef m_boxBody;

	b2RevoluteJointDef m_motor;
	b2RevoluteJointDef m_lineJoint;

	AgentBuilder(b2World& b2world, GeneticAlgorithm &GA, int x, int y) : m_b2world(b2world), m_GA(GA) {

		m_wheelFixture.shape = &m_wheelShape;
		m_wheelFixture.density = 1.0f;
		m_wheelFixture.friction = 1.0f;
		m_wheelFixture.restitution = 0.1f;
		m_wheelFixture.filter.groupIndex = -1;	// turn agent collisions off

		m_wheelBody.type = b2_dynamicBody;

		m_lineFixture.shape = &m_lineShape;
		m_lineFixture.density = 2.0f;
		m_lineFixture.friction = 1.0f;
		m_lineFixture.filter.groupIndex = -1;

		m_lineBody.type = b2_dynamicBody;

		m_boxShape.SetAsBox(0.05, 0.05);

		// perhaps use these are chromes as well
		m_boxFixture.shape = &m_boxShape;
		m_boxFixture.density = 2000.0f;
		m_boxFixture.friction = 0.1f;
		m_boxFixture.filter.groupIndex = -1;

		m_boxBody.type = b2_dynamicBody;

		m_boxBody.position.Set(x, y);

		m_motor.maxMotorTorque = 3000;
		m_motor.enableMotor = true;

		//m_lineJoint.enableLimit = true;
		m_lineJoint.enableMotor = true;
		m_lineJoint.maxMotorTorque = 9000;
	}

	b2Body* createLines(LineChromes const & gm) {

		m_lineShape.SetAsBox(gm.length * 0.5, gm.width * 0.5);

		auto body = m_b2world.CreateBody(&m_lineBody);
		body->CreateFixture(&m_lineFixture);
		m_agentParts.push_back(body);

		// no loner used
		for (auto&& wg : gm.wheel) {
			auto wheel = createWheel(wg);
			wheel->SetTransform({ gm.length, 0.0 }, 0);

			m_motor.localAnchorA = { 0.5f * gm.length, 0 };
			m_motor.bodyA = body;
			m_motor.bodyB = wheel;
			m_motor.motorSpeed = -wg.speed; // CCW

			m_b2world.CreateJoint(&m_motor);
		}

		for (auto&& sg : gm.lines) {
			auto line = createLines(sg);
			float startAngle = sg.angle;
			line->SetTransform({ gm.length, 0.0 }, startAngle);

			m_lineJoint.localAnchorA = { 0.5f * gm.length, 0 };
			m_lineJoint.localAnchorB = { -0.5f * sg.length, 0 };
			m_lineJoint.bodyA = body;
			m_lineJoint.bodyB = line;
			m_lineJoint.lowerAngle = startAngle - sg.limit;
			m_lineJoint.upperAngle = startAngle + sg.limit;
			m_lineJoint.referenceAngle = startAngle;
			m_lineJoint.motorSpeed = 1; //m_GA.generateRandomInt();
			m_b2world.CreateJoint(&m_lineJoint);
		}

		return body;
	}

	// no longer used
	b2Body* createWheel(WheelChromes const & gm) {
		m_wheelShape.m_radius = gm.radius;

		auto body = m_b2world.CreateBody(&m_wheelBody);
		body->CreateFixture(&m_wheelFixture);
		m_agentParts.push_back(body);
		return body;
	}

	b2Body* constructAgent(AgentChromes const & gm) {
		m_agentParts.clear();
		auto body = m_b2world.CreateBody(&m_boxBody);
		body->CreateFixture(&m_boxFixture);
		m_agentParts.push_back(body);

		

		for (auto&& sg : gm.lines) {
			auto line = createLines(sg);
			float startAngle = sg.angle;
			line->SetTransform({ sg.length, 0.0 }, startAngle);

			m_lineJoint.localAnchorA = { 0, 0 };
			m_lineJoint.localAnchorB = { -0.5f * sg.length, 0 };
			m_lineJoint.bodyA = body;
			m_lineJoint.bodyB = line;
			m_lineJoint.lowerAngle = startAngle - sg.limit;
			m_lineJoint.upperAngle = startAngle + sg.limit;
			m_lineJoint.referenceAngle = startAngle;

			m_b2world.CreateJoint(&m_lineJoint);
		}

		return body;
	}

};

Agent::Agent(GeneticAlgorithm* GA, b2World* b2world, int ID, int x, int y) :
	m_GA(GA),
	m_b2world(b2world),
	m_ID(ID)
{
	AgentBuilder builder(*m_b2world, *m_GA, x, y);
	m_agentBody = builder.constructAgent(m_GA->getAgentChromes());
	m_agentParts.swap(builder.m_agentParts);
}

Agent::~Agent()
{
	m_agentBody->GetWorld()->DestroyBody(m_agentBody);
}

void Agent::init(b2BodyDef &bodyDef)
{ 
	
}

void Agent::setGA(GeneticAlgorithm* GA)
{
	m_GA = GA;
}
