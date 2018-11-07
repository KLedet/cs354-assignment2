#ifndef __Ball_h_
#define __Ball_h_

#include <Ogre.h>
#include "GameObject.h"
// #include "Room.h"

//TODO: refactor using inheritance
class Ball : public GameObject{
  protected:

    //clutter
    Ogre::Vector3 bDirection;
    Ogre::Real bSpeed;
    Ogre::Real bRadius;
    Ogre::Vector3 bPosition;

  public:
    Ball();
    ~Ball();

    void init(btVector3 origin, btQuaternion rot);
    void addToSim(Simulator* mSim);
    void addToScene(Ogre::SceneManager* scnMgr);
    void update(Ogre::Real elapsedTime);
    
    Ogre::Vector3 getPosition(){ return rootNode->getPosition(); }
    Ogre::Vector3 setPosition(Ogre::Vector3 vec){ rootNode->setPosition(vec); }

    //not necessary?
    void impulse(int pNum);

    //not necessary
    btRigidBody* getBody(){return body;}

};

#endif
