#ifndef __GameObject_h_
#define __GameObject_h_

#include <Ogre.h>

#include "Simulator.h"
#include "OgreMotionState.h"

class GameObject {
protected:

	/*TODO: 
		constructor/destructor
		switch to component architecture?

	*/
	Ogre::String name;
	/* might be unnecessary? implemented in motionState
	Ogre::SceneManager* sceneMgr;
	Ogre::SceneNode* rootNode;
	Ogre::Entity* geom;*/
	OgreMotionState* motionState;

	btCollisionShape* shape;
	btRigidBody* body;
	btTransform tr;
	btVector3 inertia;

	btScalar mass;
	btScalar restitution;
	btScalar friction;
	bool kinematic; 
	bool needsUpdates;

	CollisionContext* context;
	BulletContactCallback* cCallBack;

	//should be overriden by inherited classes
	virtual void update();
public:
	GameObject(void); //TODO: params
	~GameObject(void); //clean up components
	btRigidBody* getBody(){return body;}

};

#endif