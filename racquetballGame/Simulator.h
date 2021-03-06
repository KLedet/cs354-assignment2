#ifndef __Simulator_h_
#define __Simulator_h_

#include <Ogre.h>
#include <OgreMotionState.h>


class GameObject;
class Player;
class Simulator {

protected:
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* mBroadphase;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
	std::deque<GameObject*> objList;

public:
	Simulator();
	~Simulator();
	void addRigidBody(GameObject* o);
	void addAction(Player* o);
	void addCollisionObject(btCollisionObject* o);
	bool removeObject(GameObject* o);
	void stepSimulation(const Ogre::Real elapsedTime,
	int maxSubSteps = 1, const Ogre::Real fixedTimestep = 1.0f/60.0f);
	btDiscreteDynamicsWorld* getCollisionWorld(){ return dynamicsWorld;}
};

#endif