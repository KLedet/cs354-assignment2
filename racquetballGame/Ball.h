#ifndef __Ball_h_
#define __Ball_h_

#include <Ogre.h>
#include "GameObject.h"
// #include "Room.h"

//TODO: refactor using inheritance
class Ball : public GameObject{
  protected:
    Ogre::Real bRadius;
    Ogre::Vector3 bDirection;
    Ogre::Real bSpeed;
    Ogre::SceneNode* rootNode;
    Ogre::Vector3 bPosition;

  public:
    Ball(Ogre::SceneManager* scnMgr, Simulator* sim);
    ~Ball();
    void update(Ogre::Real elapsedTime);
    void impulse();
    //not necessary
    Ogre::SceneNode* getNode() { return rootNode; }
    btRigidBody* getBody(){return body;}
    Ogre::Vector3 getPosition(){ return rootNode->getPosition(); }

    //void move(const Ogre::FrameEvent& evt);
};

#endif
