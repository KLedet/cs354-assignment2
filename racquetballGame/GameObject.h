#ifndef __GameObject_h_
#define __GameObject_h_

#include <Ogre.h>
#include "OgreMotionState.h"
#include "Simulator.h"

struct Scoreboard {
public:
    int rally[2];
    bool reset;

    Scoreboard(){
        rally[0] = 0;
        rally[1] = 0;
        reset = false;
    }
    ~Scoreboard(){}
};

class GameObject {
protected:
	Ogre::String name;
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
	

public:
	GameObject(void); 
	~GameObject(void);
	virtual void init(Ogre::SceneNode* node);
	btRigidBody* getBody(){return body;}
	bool doUpdates(){return needsUpdates;}
	virtual void update(const Ogre::Real elapsedTime);

	bool getIsKinematic(){ return isKinematic;}
	
};

#endif