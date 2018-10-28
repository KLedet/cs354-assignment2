#ifndef __Volume_h_
#define __Volume_h_

#include "GameObject.h"

class Volume : public GameObject{
private:
	btPairCachingGhostObject* triggerVolume;
	bool hit;
	bool reset;
	const btCollisionObject* collidedObj;
	btDiscreteDynamicsWorld* dynamicsWorld;
public:
	Volume(Simulator* sim, btTransform initPos);
	~Volume();
	void init(Ogre::SceneNode* node=NULL);
	void update(const Ogre::Real elapsedTime=0);
	bool hitRegistered() { return hit; }
	bool isReset() { return reset;}
	const btCollisionObject* getCollidedObject(){ return collidedObj;}
	btPairCachingGhostObject* getTriggerVolume(){ return triggerVolume;}
};

#endif
