#ifndef __Wall_h_
#define __Wall_h_

#include "GameObject.h"
#include "KillVolume.h"
class Wall : GameObject{
protected:
	KillVolume* volume;
	Scoreboard* scoreboard;
	bool kill;
public:

	Wall(Ogre::SceneManager* scnMgr, Simulator* sim, Ogre::Plane p, btQuaternion initRotation, btVector3 initPos);
	~Wall(void);
	void setScoreboard(Scoreboard* score) {scoreboard = score;}
	void update(Ogre::Real elapsedTime);
	void isKill(bool isKill){ kill = isKill;}
};

#endif