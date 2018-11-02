#ifndef __GameObject_h_
#define __GameObject_h_

#include <Ogre.h>
#include "OgreMotionState.h"
#include "Simulator.h"

//should be implemented as Game Entity
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
//How to initialize:
	//new
	//init
	//addtoscene
	//addtosim (allows to separate client/server behavior)
protected:
	Ogre::String name;

	//initialize in init()
	btTransform tr;
	OgreMotionState* motionState;
	bool needsUpdates;

	//don't know if we're using these
	CollisionContext* context;
	BulletContactCallback* cCallBack;

	//don't need both
	btRigidBody* body;
	
	btCollisionShape* shape;
public:
	GameObject(void); 
	~GameObject(void);

	bool doUpdates(){return needsUpdates;}
	//pass all this stuff in during createScene()
	virtual void init(btVector3 origin, btQuaternion rot) = 0; //actually this can probably be the constructor
	virtual void addToSim(Simulator *mSim) = 0;
	virtual void addToScene(Ogre::SceneManager *mSceneMgr) = 0;
	virtual void update(const Ogre::Real elapsedTime) = 0;
	//move this somewhere else
	btRigidBody* getBody(){return body;}
	
};

#endif