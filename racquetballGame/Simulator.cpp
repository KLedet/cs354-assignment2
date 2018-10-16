#include "Simulator.h"
#include "GameObject.h"
#include "Player.h"
#include <stdio.h>

Simulator::Simulator() {
	printf("INIT SIM\n");
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	//create overlapping pair cache
	btVector3 worldMin(-500,-500,-500);
	btVector3 worldMax(500,500,500);
	
	mBroadphase = new btAxisSweep3(worldMin,worldMax);
	mBroadphase->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,
												mBroadphase,
												solver,
												collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0.0, 0, 0.0));
	
  	
	
	//Add collision shapes to reuse among rigid bodies
}

void Simulator::addAction (Player* o){
	objList.push_back(o);
	dynamicsWorld->addCollisionObject(o->getController()->getGhostObject(), btBroadphaseProxy::CharacterFilter , btBroadphaseProxy::StaticFilter);
	dynamicsWorld->addAction(o->getController());

	// printf("player collision filter group: %d\n", o->getController()->getGhostObject()->getBroadphaseHandle()->m_collisionFilterGroup);
	// printf("player collision filter mask: %d\n", o->getController()->getGhostObject()->getBroadphaseHandle()->m_collisionFilterMask);
}
void Simulator::addRigidBody (GameObject* o) {
	objList.push_back(o);
	int filterGroup = 0;
	filterGroup |= (!o->getBody()->isStaticOrKinematicObject()) ? btBroadphaseProxy::DefaultFilter : btBroadphaseProxy::StaticFilter;

	int filterMask = btBroadphaseProxy::DefaultFilter | btBroadphaseProxy::CharacterFilter;
	filterMask |= (!o->getBody()->isStaticOrKinematicObject()) ? btBroadphaseProxy::StaticFilter | btBroadphaseProxy::SensorTrigger : 0;

	dynamicsWorld->addRigidBody(o->getBody(), filterGroup, filterMask);
	// printf("collision filter group: %d\n", o->getBody()->getBroadphaseProxy()->m_collisionFilterGroup);
	// printf("collision filter mask: %d\n", o->getBody()->getBroadphaseProxy()->m_collisionFilterMask);
	//btVector3 pos = o->getBody()->getCenterOfMassPosition();
	//printf("(%f, %f, %f)\n", pos.x(), pos.y(), pos.z());
}

void Simulator::addCollisionObject(btCollisionObject* obj) {
	objList.push_back(static_cast<GameObject*>(obj->getUserPointer()));
	int filterGroup = btBroadphaseProxy::SensorTrigger;
	int filterMask = btBroadphaseProxy::DefaultFilter;
	dynamicsWorld->addCollisionObject(obj, filterGroup, filterMask);
	// printf("Volume collision filter group: %d\n", obj->getBroadphaseHandle()->m_collisionFilterGroup);
	// printf("Volume collision filter mask: %d\n", obj->getBroadphaseHandle()->m_collisionFilterMask);
}

//Update the physics world state and any objects that have collision
void Simulator::stepSimulation(const Ogre::Real elapsedTime, int maxSubSteps, const Ogre::Real fixedTimestep) {
	//for (int i = 0; i != objList.size(); i++)
	dynamicsWorld->stepSimulation(elapsedTime, maxSubSteps, fixedTimestep);
	//handle collision here
	//iterate through objects and update()
	for (unsigned int i = 0; i < objList.size(); i++)
		if (objList[i]->doUpdates()) objList[i]->update(elapsedTime);
	
	
}