#ifndef __Room_h_
#define __Room_h_

#include <Ogre.h>
#include "Simulator.h"

class Room {
  protected:
    Ogre::SceneNode* rootNode;
    Ogre::Real bLength;
  public:
    Room(Ogre::SceneManager* scnMgr, Simulator* sim);
    ~Room();
    void addChild(Ogre::SceneNode* sNode);
    Ogre::SceneNode* getNode() { return rootNode; }
};

#endif