#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <stdlib.h>
#include "Ball.h"

Ball::Ball(Ogre::SceneManager* scnMgr) {

  // Get a new seed for rand() each time
  srand(time(0));

  //Create our ball and initialize it's variables
  Ogre::Entity* ball = scnMgr->createEntity("Sphere", Ogre::SceneManager::PT_SPHERE);
  ball->setMaterialName("Examples/SphereMappedRustySteel");
  ball->setCastShadows(true);
  rootNode = scnMgr->getRootSceneNode()->createChildSceneNode("Ball");
  rootNode->attachObject(ball);
  rootNode->scale(0.1f,0.1f,0.1f);
  bRadius = 10.0f;


  // Randomly decide which direction ball will go
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
void Ball::update(btWorldTranform &worldTrans) {
	//move this into simulator? might actually be unnecessary if the dynamicsWorld already calculates this
  // lastTime += elapsedTime;
  // simulator->getWorld()->contactTest(body, *cCallBack);
  // if (context->hit && (context->velNorm > 2.0 || context->velNorm < -2.0) 
  //   && (lastTime > 0.5 || (context->lastBody != context->body && lastTime > 0.1))) {
  //   //Handle the hit
  //   lastTime = 0.0f;
  // }
  // context->hit = false;

	//this method should update the objects position in the rendered scene using motionstates
	motionState.setWorldTransform(worldTrans);
}