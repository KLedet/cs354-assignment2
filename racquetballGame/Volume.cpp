#include "Volume.h"

Volume::Volume(btTransform initTransform){
	tr = btTransform(initTransform);

	needsUpdates = false;
}

Volume::~Volume(){
}	

void Volume::init(btVector3 origin, btQuaternion rot){
	tr.setIdentity();
	tr.setOrigin(origin);
	tr.setRotation(rot);
}

void Volume::addToSim(Simulator *mSim){
	collidedObj = NULL;
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

	dynamicsWorld = mSim->getCollisionWorld();
	mSim->addCollisionObject(triggerVolume);
}

void Volume::addToScene(Ogre::SceneManager *scnMgr){

}

void Volume::update(Ogre::Real elapsedTime){
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



			if(manifold->getNumContacts() > 0){
				collidedObj = isFirstBody ? manifold->getBody1() : manifold->getBody0();
				hit = true;
				reset = true;
				//std::cout << "contact";
				return;
			}
		}
	}
	//std::cout << "no contact" << std::endl;
	hit = false;
	collidedObj = NULL;
}
