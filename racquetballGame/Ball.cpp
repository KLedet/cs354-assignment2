#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <stdlib.h>
#include "Ball.h"

Ball::Ball(Ogre::SceneManager* scnMgr, Simulator* sim) {

  // Get a new seed for rand() each time
  srand(time(0));

  //Create our ball and initialize it's variables
  Ogre::Entity* ball = scnMgr->createEntity("Sphere", Ogre::SceneManager::PT_SPHERE);
  ball->setMaterialName("Examples/SphereMappedRustySteel");
  ball->setCastShadows(true);
  rootNode = scnMgr->getRootSceneNode()->createChildSceneNode("Ball");
  rootNode->attachObject(ball);
  rootNode->scale(0.2f,0.2f,0.2f);
  bRadius = 10.0f;

  	tr.setIdentity();
	tr.setRotation(btQuaternion(1.0f, 1.0f, 1.0f, 0));
	tr.setOrigin(btVector3(0,0,-30));
	shape = new btSphereShape(bRadius);
  this->init(rootNode);

  Ogre::Real x_val = (Ogre::Real)(rand() % 8 + 50);
  if(rand() % 3 == 1)
    x_val *= -1;

  body->setLinearVelocity(btVector3(x_val, 100, -300));
  



  needsUpdates = false;
  sim->addRigidBody(this);
  // Randomly decide which direction ball will go
  /*
  Ogre::Real x_val = (Ogre::Real)(rand() % 8 + 1);
  if(rand() % 3 == 1)
	x_val *= -1;
  Ogre::Real y_val = (Ogre::Real)(rand() % 8 + 1);
  if(rand() % 3 == 1)
	y_val *= -1;
  Ogre::Real z_val = (Ogre::Real)(rand() % 8 + 1);
  if(rand() % 3 == 1)
	z_val *= -1;

  // Tell ball which direction to move
  bDirection = Ogre::Vector3(x_val, y_val, z_val);
  bDirection.normalise();
  bSpeed = (Ogre::Real)(rand() % 200 + 100);
  bPosition = rootNode->getPosition();
  */

}

void Ball::update(Ogre::Real elapsedTime){
	btTransform transform;
	
	transform.setRotation(btQuaternion(1.0f,1.0f,1.0f,0));
	transform.setOrigin(btVector3(0,0,-30));
	body->setWorldTransform(transform);
	motionState->setWorldTransform(transform);

  srand(time(0));

  Ogre::Real x_val = (Ogre::Real)(rand() % 8 + 50);
  if(rand() % 3 == 1)
    x_val *= -1;
	body->setLinearVelocity(btVector3(x_val,100,-300));


}

void Ball::impulse(){
	printf("impulse\n");
	body->applyCentralImpulse(btVector3(0, 0, -20));
	body->activate();
}