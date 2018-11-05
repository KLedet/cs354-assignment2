#ifndef __Player_h_
#define __Player_h_

#include "GameObject.h"
#include "Volume.h"
#include "Ball.h"

class Player : public GameObject{

	//TODO: attach the camera to the player
private:
	btVector3 mVelocity;
	Ogre::SceneNode* rootNode;
	btKinematicCharacterController* controller;
	btDiscreteDynamicsWorld* collisionWorld;
	Volume* volume;
	btQuaternion rot_0;
	btQuaternion rot_1;
	Ogre::Real last_time;
	bool isSwinging;
	float swingSpeed;
	int playerNum;

public:
	
	Player(Ogre::SceneManager* scnMgr, Simulator* sim, int pNum);
	~Player(void);

	void init(btVector3 origin, btQuaternion rot);
	void addToSim(Simulator *mSim);
	void addToScene(Ogre::SceneManager *mSceneMgr);
	void update(const Ogre::Real elapsedTime);
	
	void input(btVector3 newVelocity);
	void swing();
	void unswing();
	Ogre::Vector3 getPosition(){ return rootNode->getPosition(); }
	Ogre::Quaternion getOrientation(){ return rootNode->getOrientation(); }
	Ogre::SceneNode* getSceneNode(){return rootNode;}
	//This is really messy and needs to be changed so we can despaghettify the code
	//we will need to implement single player back in so keep this in mind
	void setPosition(btVector3 vec);
	void setRotation2();
	void setOgrePosition(Ogre::Vector3 vec){rootNode->setPosition(vec);}
	void setOgreRotation(Ogre::Quaternion rot){rootNode->setOrientation(rot);}
	btKinematicCharacterController* getController(){ return controller;}
	btVector3 getVelocity(){ return mVelocity; }
};
#endif
