#ifndef __Player_h_
#define __Player_h_

#include "GameObject.h"

class Player : GameObject{

	//TODO: attach the camera to the player
private:
	btVector3 mVelocity;
	Ogre::SceneNode* rootNode;
public:
	void input(btVector3 newVelocity);
	Player(Ogre::SceneManager* scnMgr, Simulator* sim);
	~Player(void){}
	void update(Ogre::Real elapsedTime);
	Ogre::Vector3 getPosition(){ return rootNode->getPosition(); };
};
#endif
