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
	tr.setOrigin(btVector3(0,0,0));
	shape = new btSphereShape(bRadius);
  this->init(rootNode);
  body->setLinearVelocity(btVector3(0, 0, -100));
  
  sim->addObject(this);
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

/* replace with physics
void Ball::move(const Ogre::FrameEvent& evt){

	  // Move ball and check for collision where our room will be occupying
	if(rootNode->getPosition().x < 250 && rootNode->getPosition().x > -250 
		&& rootNode->getPosition().y < 250 && rootNode->getPosition().y > -250
		&& rootNode->getPosition().z < 250 && rootNode->getPosition().z > -250)
	{
		rootNode->translate(bSpeed * evt.timeSinceLastFrame * bDirection);
	}
	if(rootNode->getPosition().x >= 250 || rootNode->getPosition().x <= -250)
	{
		bDirection.x *= -1;
		rootNode->translate(bSpeed * evt.timeSinceLastFrame * bDirection);
	}
	if(rootNode->getPosition().y >= 250 || rootNode->getPosition().y <= -250)
	{
		bDirection.y *= -1;
		rootNode->translate(bSpeed * evt.timeSinceLastFrame * bDirection);
	}
	if(rootNode->getPosition().z >= 250 || rootNode->getPosition().z <= -250)
	{
		bDirection.z *= -1;
		rootNode->translate(bSpeed * evt.timeSinceLastFrame * bDirection);
	}

	bPosition = rootNode->getPosition();
}
*/
