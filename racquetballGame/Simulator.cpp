#include "Simulator.h"
#include "GameObject.h"
#include <stdio.h>
Simulator::Simulator() {
	printf("INIT SIM\n");
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,
												overlappingPairCache,
												solver,
												collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0.0, 0, 0.0));
	objList.clear();
	//Add collision shapes to reuse among rigid bodies
}

void Simulator::addObject (GameObject* o) {
	objList.push_back(o);
	dynamicsWorld->addRigidBody(o->getBody());
	btVector3 pos = o->getBody()->getCenterOfMassPosition();
	//printf("(%f, %f, %f)\n", pos.x(), pos.y(), pos.z());
	
}

//Update the physics world state and any objects that have collision
void Simulator::stepSimulation(const Ogre::Real elapsedTime, int maxSubSteps, const Ogre::Real fixedTimestep) {
	//for (int i = 0; i != objList.size(); i++)
	dynamicsWorld->stepSimulation(elapsedTime, maxSubSteps, fixedTimestep);
	//handle collision here
	//iterate through objects and update()
	// for (unsigned int i = 0; i < objList.size(); i++)
	//   if (objList[i]->gObject->doUpdates()) objList[i]->gObject->update(elapsedTime);
	//
	for (unsigned int i = 0; i < objList.size(); i++) {
		//hitcode
		if(objList[i]->getIsKinematic()){
			objList[i]->update(elapsedTime);
		}
	}
}