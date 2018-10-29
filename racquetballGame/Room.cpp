#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include "Room.h"
#include "Wall.h"

Room::Room(Ogre::SceneManager* scnMgr, Simulator* sim, Scoreboard* score){
    Ogre::Plane plane( Ogre::Vector3::UNIT_Z, 0 );

    Ogre::Real pSize = 500;

    // Create our walls
    Ogre::MeshManager::getSingleton().createPlane("backwall",
                            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                            plane, pSize,pSize,1,1, true, 1, 5, 5,
                            Ogre::Vector3::UNIT_Y);
    Ogre::MeshManager::getSingleton().createPlane("wall",
                            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                            plane, pSize,pSize*2,1,1, true, 1, 5, 5,
                            Ogre::Vector3::UNIT_Y);

    btQuaternion q(0, - SIMD_PI / 2, 0);
    Wall* w1 = new Wall(scnMgr, sim, plane, q, btVector3(0, -250, 0));
    q = btQuaternion(0, SIMD_PI / 2, 0);
    Wall* w2 = new Wall(scnMgr, sim, plane, q, btVector3(0, 250, 0));
    
    btQuaternion pitch(0, -SIMD_PI / 2, 0);
    q = btQuaternion(SIMD_PI / 2, 0, 0);
    Wall* w3 = new Wall(scnMgr, sim, plane, pitch * q, btVector3(-250, 0, 0));
   
    q = btQuaternion(- SIMD_PI / 2, 0, 0);
    Wall* w4 = new Wall(scnMgr, sim, plane, pitch * q, btVector3(250,0, 0));
    
    q = btQuaternion(SIMD_PI, 0, 0);
    Wall* w5 = new Wall(scnMgr, sim, plane, q, btVector3(0,0, 500), true);


    //kill wall
    w5->isKill(true);
    w5->setScoreboard(score);
    //identity, scoring wall
    q = btQuaternion(0,0,0,1.0f);
    Wall* w6= new Wall(scnMgr, sim, plane, q, btVector3(0, 0, -500), true);
    w6->setScoreboard(score);
    w6->isKill(true);
    

}

void Room::addChild(Ogre::SceneNode* sNode){
  rootNode->addChild(sNode);
}