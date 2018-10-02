#include <Ogre.h>


class Room {
  protected:
    Ogre::SceneNode* rootNode;
    Ogre::Real bLength;
  public:
    Room(Ogre::SceneManager* scnMgr);
    ~Room();
    void addChild(Ogre::SceneNode* sNode);
    Ogre::SceneNode* getNode() { return rootNode; }
};