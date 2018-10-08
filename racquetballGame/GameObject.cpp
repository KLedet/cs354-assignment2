#include "GameObject.h"

 //commented out for now bc the game object probably doesn't need to have any interaction with the simulator
	//But the rigid body code will probably end up in the constructor

GameObject::GameObject(void) :
	motionState(0),
	body(0),
	shape(0),
	mass(1),
	friction(0),
	restitution(1),
	isKinematic(true)
	{
	//construct motionState
}


GameObject::~GameObject(void){
}

void GameObject::init(btVector3 initPos, Ogre::SceneNode* node){
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(1.0f, 1.0f, 1.0f, 0));
	startTransform.setOrigin(initPos);
	shape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));
	inertia = btVector3(0,0,0);
	motionState = new OgreMotionState(startTransform, node);
	motionState->setWorldTransform(startTransform);
	 if (mass != 0.0f) shape->calculateLocalInertia(mass, inertia);
  btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, inertia);
  rbInfo.m_restitution = restitution;
  rbInfo.m_friction = friction;
  body = new btRigidBody(rbInfo);
  body->setUserPointer(this);
  if (isKinematic) {
    body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    body->setActivationState(DISABLE_DEACTIVATION);
  }
}
//Add the game object to the simulator
void GameObject::addToSimulator(Simulator* simulator) {
   if (mass != 0.0f) shape->calculateLocalInertia(mass, inertia);
  btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, inertia);
  rbInfo.m_restitution = restitution;
  rbInfo.m_friction = friction;
  body = new btRigidBody(rbInfo);
  body->setUserPointer(this);
  if (isKinematic) {
    body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    body->setActivationState(DISABLE_DEACTIVATION);
  }
  simulator->addObject(this);
}

void GameObject::update(btTransform &worldTrans) {
	//move this into simulator? might actually be unnecessary if the dynamicsWorld already calculates this
	  /*lastTime += elapsedTime;
	  simulator->getWorld()->contactTest(body, *cCallBack);
	  if (context->hit && (context->velNorm > 2.0 || context->velNorm < -2.0) 
	    && (lastTime > 0.5 || (context->lastBody != context->body && lastTime > 0.1))) {
	    //Handle the hit
	    lastTime = 0.0f;
	  }
	  context->hit = false;*/

	//this method should update the objects position in the rendered scene using motionstates
	motionState->setWorldTransform(worldTrans);
}