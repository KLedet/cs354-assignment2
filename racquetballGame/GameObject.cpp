#include "GameObject.h"

 //commented out for now bc the game object probably doesn't need to have any interaction with the simulator
	//But the rigid body code will probably end up in the constructor

GameObject::GameObject(void) :
	motionState(0),
	body(0),
	shape(0) {
}


GameObject::~GameObject(void){
}

void GameObject::update(Ogre::Real elapsedTime){
	printf("you really shouldn't be here\n");
}