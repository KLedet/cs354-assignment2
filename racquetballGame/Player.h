#ifndef __Player_h_
#define __Player_h_

#include "GameObject.h"

class Player : GameObject{
private:
	btTransform mVelocity;
public:
	void input(btTransform &newVelocity);
};
#endif
