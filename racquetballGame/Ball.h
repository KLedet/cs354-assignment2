#ifndef __Ball_h_
#define __Ball_h_

#include <Ogre.h>
#include "GameObject.h"
// #include "Room.h"

//TODO: refactor using inheritance
class Ball : GameObject{
  protected:
    Ogre::Real bRadius;
    Ogre::Vector3 bDirection;
    Ogre::Real bSpeed;
    Ogre::SceneNode* rootNode;
    Ogre::Vector3 bPosition;

  public:
    Ball(Ogre::SceneManager* scnMgr, Simulator* sim);
    ~Ball();
    void update(btTransform &worldTrans);


    //not necessary
    Ogre::SceneNode* getNode() { return rootNode; }
    Ogre::Vector3 getPosition(){ return bPosition; }
    //void move(const Ogre::FrameEvent& evt);
};

#endif
