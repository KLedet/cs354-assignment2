#include "KillVolume.h"

KillVolume::KillVolume(Simulator* sim, btTransform initTransform){
	tr = btTransform(initTransform);

	init();
	dynamicsWorld = sim->getCollisionWorld();
	sim->addCollisionObject(triggerVolume);
	needsUpdates = false;
}

KillVolume::~KillVolume(){

}

void KillVolume::init(Ogre::SceneNode* node){
	hit = false;
	reset = false;
	triggerVolume = new btPairCachingGhostObject();
	

	btTransform startTransform = tr;

	btConvexShape* convexShape = new btBoxShape(btVector3(250.0f,250.0f, 10.0f));
	triggerVolume->setWorldTransform(startTransform);
	triggerVolume->setCollisionShape( convexShape);
	triggerVolume->setCollisionFlags(triggerVolume->getCollisionFlags() |
    	btCollisionObject::CF_NO_CONTACT_RESPONSE);
	triggerVolume->setUserPointer(this);

}

void KillVolume::update(Ogre::Real elapsedTime){

	//don't need elapse time for this
	btManifoldArray manifoldArray;
	btBroadphasePairArray& pairArray =
		triggerVolume->getOverlappingPairCache()->getOverlappingPairArray();
	int numPairs = pairArray.size();
	for (int i = 0; i < numPairs; ++i)
	{
		manifoldArray.clear();

		const btBroadphasePair& pair = pairArray[i];

		btBroadphasePair* collisionPair =
			dynamicsWorld->getPairCache()->findPair(
				pair.m_pProxy0,pair.m_pProxy1);

		if (!collisionPair) continue;

		if (collisionPair->m_algorithm)
			collisionPair->m_algorithm->getAllContactManifolds(manifoldArray);

		for (int j = 0; j < manifoldArray.size(); j++)
		{
			btPersistentManifold* manifold = manifoldArray[j];

			bool isFirstBody = manifold->getBody0() == triggerVolume;

			btScalar direction = isFirstBody ? btScalar(-1.0) : btScalar(1.0);

			if(manifold->getNumContacts() > 0){
				hit = true;
				return;
			}
		}
	}
	if(reset){
		reset = false;
	}
	hit = false;
}

