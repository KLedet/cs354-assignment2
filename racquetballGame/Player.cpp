#include "Player.h"

void Player::input(btTransform &newVelocity){
	mVelocity = newVelocity;
	//TODO: rotate with player's orientation
	body->setLinearVelocity(mVelocity);
}

