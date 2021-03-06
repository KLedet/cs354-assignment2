#include "GameObject.h"

 //commented out for now bc the game object probably doesn't need to have any interaction with the simulator
	//But the rigid body code will probably end up in the constructor

GameObject::GameObject(void) :
	motionState(0),
	body(0),
	shape(0),
	mass(1),
	friction(0),
	restitution(0.9f),
	isKinematic(false) {
}


GameObject::~GameObject(void){
}

void GameObject::init(Ogre::SceneNode* node){
	btTransform startTransform = tr;
	
	if (!shape)
		shape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));
	inertia = btVector3(0,0,0);
	motionState = new OgreMotionState(startTransform, node);
	motionState->setWorldTransform(startTransform);
	if (mass != 0.0f) 
		shape->calculateLocalInertia(mass, inertia);
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

void GameObject::update(Ogre::Real elapsedTime){
	printf("you really shouldn't be here\n");
}