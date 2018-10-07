#include "Player.h"

void Player::input(btVector3 &newVelocity){
	mVelocity = newVelocity;
	//TODO: rotate with player's orientation
	body->setLinearVelocity(mVelocity);
}

