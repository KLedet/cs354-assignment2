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
	Volume(btTransform initPos);
	~Volume();
	void init(btVector3 origin, btQuaternion rot); //static objects will not need to have their positions updates afterwards
	void addToSim(Simulator *mSim);
	void addToScene(Ogre::SceneManager *mSceneMgr);
	void update(const Ogre::Real elapsedTime=0);

	bool hitRegistered() { return hit; }
	bool isReset() { return reset;}
	const btCollisionObject* getCollidedObject(){ return collidedObj;}
	btPairCachingGhostObject* getTriggerVolume(){ return triggerVolume;}
};

#endif
