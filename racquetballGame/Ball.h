#ifndef __Ball_h_
#define __Ball_h_

#include <Ogre.h>
#include "GameObject.h"
// #include "Room.h"

//TODO: refactor using inheritance
class Ball {
  protected:
    Ogre::Real bRadius;
    Ogre::Vector3 bDirection;
    Ogre::Real bSpeed;
    Ogre::Vector3 bPosition;
    Ogre::SceneNode* rootNode;
    
  public:
    Ball(Ogre::SceneManager* scnMgr);
    ~Ball();
    void move(const Ogre::FrameEvent& evt);
    Ogre::SceneNode* getNode() { return rootNode; }
    Ogre::Vector3 getPosition(){ return bPosition; }
    void update(float elapsedTime);
};

#endif