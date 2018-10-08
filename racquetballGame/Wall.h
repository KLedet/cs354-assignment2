#ifndef __Wall_h_
#define __Wall_h_

#include "GameObject.h"

class Wall : GameObject{
public:
	Wall(Ogre::SceneManager* scnMgr, Simulator* sim, Ogre::Plane p, btQuaternion initRotation, btVector3 initPos);
	~Wall(void);
};

#endif