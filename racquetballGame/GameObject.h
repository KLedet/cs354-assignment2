#ifndef __GameObject_h_
#define __GameObject_h_

#include <Ogre.h>
#include "OgreMotionState.h"
#include "Simulator.h"

class GameObject {
protected:

	/*TODO: 
		constructor/destructor

	*/
	Ogre::String name;
	/* might be unnecessary? implemented in motionState
	Ogre::SceneManager* sceneMgr;
	Ogre::SceneNode* rootNode;
	Ogre::Entity* geom;*/
	OgreMotionState* motionState;
	btRigidBody* body;
	
	btCollisionShape* shape;
	
	btTransform tr;
	btVector3 inertia;

	btScalar mass;
	btScalar restitution;
	btScalar friction;
	bool isKinematic;  
	bool needsUpdates;
	
	CollisionContext* context;
	BulletContactCallback* cCallBack;
	void addToSimulator(Simulator* simulator);
	

public:

	//TODO:
	//initialization
	//initialize rigid body
	GameObject(void); //should be called in the initialization list
	~GameObject(void); //clean up components
	virtual void init(Ogre::SceneNode* node);
	btRigidBody* getBody(){return body;}
	bool doUpdates(){return needsUpdates;}
	//should just synchronize world representations for renderer and phys engine
	virtual void update(const Ogre::Real elapsedTime);

	bool getIsKinematic(){ return isKinematic;}
	
};

#endif