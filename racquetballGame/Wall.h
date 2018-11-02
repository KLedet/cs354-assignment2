#ifndef __Wall_h_
#define __Wall_h_

#include "GameObject.h"
#include "Volume.h"
class Wall : GameObject{
protected:
	Volume* volume;
	Scoreboard* scoreboard;
	Ogre::SceneNode* rootNode;
	bool kill;
	bool isActive;
	bool isBackwall; //use inheritance
	Ogre::Plane plane; //Don't think we actually use this
	int id;
public:
	Wall(Ogre::Plane p, btQuaternion rot, btVector3 origin, bool backwall=false);
	~Wall(void);

	void init(btVector3 origin, btQuaternion rot);
	void addToSim(Simulator *mSim);
	void addToScene(Ogre::SceneManager *mSceneMgr);
	void update(const Ogre::Real elapsedTime);

	int getID(){return id;}
	void setID(int newID){id = newID;}
	void setScoreboard(Scoreboard* score) {scoreboard = score;}
	void isKill(bool isKill){ kill = isKill;}
};

#endif