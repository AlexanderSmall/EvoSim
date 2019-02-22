#include "contactlistener.h"


ContactListener::ContactListener(World *pWorld) : m_world(pWorld) {
	m_massData = new b2MassData();
	m_manifold = new b2WorldManifold();
}

ContactListener::~ContactListener() {
	delete m_massData;
	delete m_manifold;
}

void ContactListener::PostSolve(b2Contact* contact,
	const b2ContactImpulse* impulse) {
	b2Fixture *fixture = contact->GetFixtureB();
	int indexFixture = -1;
	b2Fixture *motoFixture;
	/* // UNCOMMENT WHEN COLLISION IS NEEDED 
	Agent *agent = m_world->getAgent();
	for (int i = 0; i < 8; i++) {
		motoFixture = agent->getCartFixture(i);
		if (fixture == motoFixture) {
			indexFixture = i;
			break;
		}
		motoFixture = car->getAxleFixture(i);
		if (fixture == motoFixture) {
			indexFixture = i + 8;
			break;
		}
	}
	if (indexFixture < 0)
		return;
	int pointCount = contact->GetManifold()->pointCount;
	float maxImpulses = 0;
	contact->GetWorldManifold(manifold);
	for (int i = 0; i < pointCount; i++) {
		
	}
	motoFixture->GetMassData(massData);
	float strength = 50 * massData->mass;
	if (strength < maxImpulses)
		car->setBreakFixture(indexFixture, true);
	*/
}
