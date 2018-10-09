#include "Player.h"

Player::Player(Ogre::SceneManager* scnMgr, Simulator* sim){
	mVelocity = btVector3(0,0,0);
	isKinematic = true;
	mass = 10.0f;
	shape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));
	Ogre::Entity* paddle = scnMgr->createEntity("cube.mesh");
  	paddle->setMaterialName("Examples/OgreLogo");
  	paddle->setCastShadows(true);
  	rootNode = scnMgr->getRootSceneNode()->createChildSceneNode("Cubo");
  	rootNode->attachObject(paddle);
  	rootNode->scale(0.8f,0.4f,0.05f);
  	tr.setIdentity();
	tr.setRotation(btQuaternion(0,0,0,1.0f));
	tr.setOrigin(btVector3(10, 0, 0));
  	init(rootNode);
  	sim->addObject(this);
}
void Player::input(btVector3 newVelocity){

	mVelocity = newVelocity;
	//TODO: rotate with player's orientation
	body->setLinearVelocity(mVelocity);
}

void Player::update(Ogre::Real elapsedTime){
	printf("%f | ", elapsedTime);
	btVector3 dist = elapsedTime * 100.0 * mVelocity;
	printf("dist: %f %f %f\n", dist.x(), dist.y(), dist.z());
	btTransform trans;
	motionState->getWorldTransform(trans);
	btVector3 origin = trans.getOrigin();

	trans.setOrigin(trans.getOrigin() + dist);
	motionState->setWorldTransform(trans);
}
