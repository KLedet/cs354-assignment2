#include <Ogre.h>
// #include "Room.h"

class Ball {
  protected:
    Ogre::SceneNode* rootNode;
    Ogre::Real bRadius;
    Ogre::Vector3 bDirection;
    Ogre::Real bSpeed;
    Ogre::Vector3 bPosition;
    // Room * room;
  public:
    Ball(Ogre::SceneManager* scnMgr);
    ~Ball();
    void move(const Ogre::FrameEvent& evt);
    Ogre::SceneNode* getNode() { return rootNode; }
    Ogre::Vector3 getPosition(){ return bPosition; }
    void update(float elapsedTime);
    // void setRoom(Room * r) { room = r; }
};