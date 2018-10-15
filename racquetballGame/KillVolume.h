#ifndef __KillVolume_h_
#define __KillVolume_h_

#include "GameObject.h"

class KillVolume : public GameObject{
private:
	btPairCachingGhostObject* triggerVolume;
	bool hit;
	bool reset;
	btDiscreteDynamicsWorld* dynamicsWorld;
public:
	KillVolume(Simulator* sim, btTransform initPos);
	~KillVolume();
	void init(Ogre::SceneNode* node=NULL);
	void update(const Ogre::Real elapsedTime=0);
	bool hitRegistered() { return hit; }
	bool volumeReset() { return reset;}
};

#endif