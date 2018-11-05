#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include "Room.h"
#include "Wall.h"

Room::Room(Ogre::SceneManager* scnMgr, Simulator* sim, Scoreboard* score){

    //this class serves little purpose, but it's fine
    //we can derive the class actually to do the setup!
    //That way we can easily make it single or multiplayer
    //maybe even turn this into a gameworld class that we pass an engine context into??
    //so we can just query it about positions of dynamic objects etc
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
    Wall* w1 = new Wall(q, btVector3(0, -250, 0));
    w1->addToScene(scnMgr);
    w1->addToSim(sim);

    q = btQuaternion(0, SIMD_PI / 2, 0);
    Wall* w2 = new Wall(q, btVector3(0, 250, 0));
    w2->addToScene(scnMgr);
    w2->addToSim(sim);
    
    btQuaternion pitch(0, -SIMD_PI / 2, 0);
    q = btQuaternion(SIMD_PI / 2, 0, 0);
    Wall* w3 = new Wall(pitch * q, btVector3(-250, 0, 0));
    w3->addToScene(scnMgr);
    w3->addToSim(sim);   

    q = btQuaternion(- SIMD_PI / 2, 0, 0);
    Wall* w4 = new Wall(pitch * q, btVector3(250,0, 0));
    w4->addToScene(scnMgr);
    w4->addToSim(sim);

    q = btQuaternion(SIMD_PI, 0, 0);
    Wall* w5 = new Wall(q, btVector3(0,0, 500), true);

    w5->addToScene(scnMgr);
    w5->addToSim(sim);

    //kill wall
    w5->isKill(true);
    w5->setScoreboard(score);
    w5->setID(0);
    //identity, scoring wall
    q = btQuaternion(0,0,0,1.0f);
    Wall* w6= new Wall(q, btVector3(0, 0, -500), true);
    w6->addToScene(scnMgr);
    w6->addToSim(sim);
    w6->setScoreboard(score);
    w6->setID(1);
    w6->isKill(true);
    

}