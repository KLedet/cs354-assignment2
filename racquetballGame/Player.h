#ifndef __Player_h_
#define __Player_h_

#include "GameObject.h"

class Player : GameObject{

	//TODO: attach the camera to the player
private:
	btVector3 mVelocity;
public:
	void input(btVector3 newVelocity);
	Player(Ogre::SceneManager* scnMgr, Simulator* sim);
	~Player(void){}
	void update(Ogre::Real elapsedTime);
};
#endif
