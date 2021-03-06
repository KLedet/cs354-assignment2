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

public:
	void input(btVector3 newVelocity);
	Player(Ogre::SceneManager* scnMgr, Simulator* sim);
	~Player(void){}
	void init(Ogre::SceneNode* node);
	void update(Ogre::Real elapsedTime);
	void swing();
	void unswing();
	Ogre::Vector3 getPosition(){ return rootNode->getPosition(); }
	btKinematicCharacterController* getController(){ return controller;}
};
#endif
