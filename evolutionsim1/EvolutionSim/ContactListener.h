#ifndef CONTACTLISTENER_H
#define CONTACTLISTENER_H

#include <Box2d/Box2D.h>
#include "world.h"

class World;

class ContactListener : public b2ContactListener
{
public:
	ContactListener(World *pWorld);
	~ContactListener();

private:
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

	b2MassData *m_massData;
	b2WorldManifold *m_manifold;
	World *m_world;
};

#endif // CONTACTLISTENER_H
