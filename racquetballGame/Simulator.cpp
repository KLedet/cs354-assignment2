#include "Simulator.h"

Simulator::Simulator() {
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,
												overlappingPairCache,
												solver,
												collisionConfiguration);
												dynamicsWorld->setGravity(btVector3(0.0, -0.098, 0.0));
	//Add collision shapes to reuse among rigid bodies
}

void Simulator::addObject (GameObject* o) {
	objList.push_back(o);
	dynamicsWorld->addRigidBody(o->getBody());
}

//Update the physics world state and any objects that have collision
void Simulator::stepSimulation(const Ogre::Real elapsedTime, int maxSubSteps, const Ogre::Real fixedTimestep) {
	for (int i = 0; i != objList.size(); i++)
		dynamicsWorld->stepSimulation(elapsedTime, maxSubSteps, fixedTimestep);
	// for (unsigned int i = 0; i < objList.size(); i++)
	//   if (objList[i]->gObject->doUpdates()) objList[i]->gObject->update(elapsedTime);
}