#ifndef __KillVolume_h_
#define __KillVolume_h_

#include "GameObject.h"

class KillVolume : public GameObject{
private:
	btPairCachingGhostObject* triggerVolume;
	bool hit;
	btDiscreteDynamicsWorld* dynamicsWorld;
public:
	KillVolume(Simulator* sim, btVector3 initPos);
	~KillVolume();
	void init(Ogre::SceneNode* node=NULL);
	void update(const Ogre::Real elapsedTime=0);
	bool hitRegistered() { return hit; }
};

#endif