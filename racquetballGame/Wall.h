#ifndef __Wall_h_
#define __Wall_h_

#include "GameObject.h"
#include "Volume.h"
class Wall : GameObject{
protected:
	Volume* volume;
	Scoreboard* scoreboard;
	bool kill;
	bool isActive;
	int id;
public:
	int getID(){return id;}
	void setID(int newID){id = newID;}
	Wall(Ogre::SceneManager* scnMgr, Simulator* sim, Ogre::Plane p, btQuaternion initRotation, btVector3 initPos, bool backwall=false);
	~Wall(void);
	void setScoreboard(Scoreboard* score) {scoreboard = score;}
	void update(Ogre::Real elapsedTime);
	void isKill(bool isKill){ kill = isKill;}
};

#endif