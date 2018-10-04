#include "GameObject.h"

/* commented out for now bc the game object probably doesn't need to have any interaction with the simulator
	But the rigid body code will probably end up in the constructor

//Add the game object to the simulator
void GameObject::addToSimulator() {
  //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
  //updateTransform();
  //rigidbody is dynamic if and only if mass is non zero, otherwise static
  if (mass != 0.0f) shape->calculateLocalInertia(mass, inertia);
  btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, inertia);
  rbInfo.m_restitution = restitution;
  rbInfo.m_friction = friction;
  body = new btRigidBody(rbInfo);
  body->setUserPointer(this);
  if (kinematic) {
    body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    body->setActivationState(DISABLE_DEACTIVATION);
  }
  simulator->addObject(this);
}
*/

GameObject::GameObject(void){

}

GameObject::~GameObject(void){
}