#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <stdlib.h>
#include "Ball.h"

Ball::Ball() {

  // Get a new seed for rand() each time
  

  
  bRadius = 10.0f;

  tr.setIdentity();
	tr.setRotation(btQuaternion(1.0f, 1.0f, 1.0f, 0));
	tr.setOrigin(btVector3(0,0,-30));
	shape = new btSphereShape(bRadius);

  needsUpdates = false;
  
}

void Ball::init(btVector3 origin, btQuaternion rot){
  tr.setIdentity();
  tr.setRotation(rot);
  tr.setOrigin(origin);
  
}

void Ball::addToScene(Ogre::SceneManager* scnMgr){
  //make the motionState, sceneNode, etc here
  Ogre::Entity* ball = scnMgr->createEntity("Sphere", Ogre::SceneManager::PT_SPHERE);
  ball->setMaterialName("Examples/SphereMappedRustySteel");
  ball->setCastShadows(true);
  rootNode = scnMgr->getRootSceneNode()->createChildSceneNode("Ball");
  rootNode->attachObject(ball);
  rootNode->scale(0.2f,0.2f,0.2f);

  motionState = new OgreMotionState(tr, rootNode);

}

void Ball::addToSim(Simulator* mSim){
  //put the rigid body code and add to sim here
  if (!shape)
    shape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));
  btVector3 inertia = btVector3(0,0,0);
  btScalar friction = 0;
  btScalar restitution = .9f;
  btScalar mass = 1.0f;

  if (mass != 0.0f) 
    shape->calculateLocalInertia(mass, inertia);


  //instantiate rigid body
  btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, inertia);
  rbInfo.m_restitution = restitution;
  rbInfo.m_friction = friction;
  body = new btRigidBody(rbInfo);
  body->setUserPointer(this);

  //add to simulator
  mSim->addRigidBody(this);

  srand(time(0));
  Ogre::Real x_val = (Ogre::Real)(rand() % 8 + 50);
  if(rand() % 3 == 1)
    x_val *= -1;
  
  body->setLinearVelocity(btVector3(x_val, 100, -300));
}


//bootleg event system
void Ball::update(Ogre::Real elapsedTime){
	btTransform transform;
	
	transform.setRotation(btQuaternion(1.0f,1.0f,1.0f,0));
	transform.setOrigin(btVector3(0,0,-30));
  tr = transform;
	body->setWorldTransform(transform);
	motionState->setWorldTransform(transform);

  srand(time(0));

  Ogre::Real x_val = (Ogre::Real)(rand() % 8 + 50);
  if(rand() % 3 == 1)
    x_val *= -1;
	body->setLinearVelocity(btVector3(x_val,100,-300));


}

void Ball::impulse(int pNum){
	printf("impulse\n");
  if(pNum == 1)
    body->applyCentralImpulse(btVector3(0, 0, -20));
  else if(pNum == 2)
    body->applyCentralImpulse(btVector3(0, 0, 20));
	body->activate();
}