#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include "Room.h"

Room::Room(Ogre::SceneManager* mSceneMgr) {
    // Set our texture for the walls
    std::string material_str = "Examples/BumpyMetal";

    // Create ground
    Ogre::Plane plane( Ogre::Vector3::UNIT_Y, -250 );
    Ogre::MeshManager::getSingleton().createPlane("ground",
    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
    500,500,1,1,true,1,5,5,Ogre::Vector3::UNIT_Z);
    Ogre::Entity* ent = mSceneMgr->createEntity( "GroundEntity", "ground" );
    rootNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    rootNode->attachObject(ent);
    ent->setMaterialName(material_str);
    ent->setCastShadows(true);

    // Create ceiling/roof
    Ogre::Plane plane2( Ogre::Vector3::NEGATIVE_UNIT_Y, -250 );
    Ogre::MeshManager::getSingleton().createPlane("roof",
    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane2,
    500,500,1,1,true,1,5,5,Ogre::Vector3::UNIT_Z);
    ent = mSceneMgr->createEntity( "RoofEntity", "roof" );
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent);
    ent->setMaterialName(material_str);
    ent->setCastShadows(true);

    // These next 4 make the surrounding walls
    Ogre::Plane plane3( Ogre::Vector3::UNIT_Z, -250 );
    Ogre::MeshManager::getSingleton().createPlane("wall1",
    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane3,
    500,500,1,1,true,1,5,5,Ogre::Vector3::UNIT_X);
    ent = mSceneMgr->createEntity( "WallEntity1", "wall1" );
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent);
    ent->setMaterialName(material_str);
    ent->setCastShadows(true);

    Ogre::Plane plane4( Ogre::Vector3::NEGATIVE_UNIT_Z, -250 );
    Ogre::MeshManager::getSingleton().createPlane("wall2",
    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane4,
    500,500,1,1,true,1,5,5,Ogre::Vector3::UNIT_X);
    ent = mSceneMgr->createEntity( "WallEntity2", "wall2" );
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent);
    ent->setMaterialName(material_str);
    ent->setCastShadows(true);

    Ogre::Plane plane5( Ogre::Vector3::UNIT_X, -250 );
    Ogre::MeshManager::getSingleton().createPlane("wall3",
    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane5,
    500,500,1,1,true,1,5,5,Ogre::Vector3::UNIT_Z);
    ent = mSceneMgr->createEntity( "WallEntity3", "wall3" );
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent);
    ent->setMaterialName(material_str);
    ent->setCastShadows(true);

    Ogre::Plane plane6( Ogre::Vector3::NEGATIVE_UNIT_X, -250 );
    Ogre::MeshManager::getSingleton().createPlane("wall4",
    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane6,
    500,500,1,1,true,1,5,5,Ogre::Vector3::UNIT_Z);
    ent = mSceneMgr->createEntity( "WallEntity4", "wall4" );
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent);
    ent->setMaterialName(material_str);
    ent->setCastShadows(true);
}

void Room::addChild(Ogre::SceneNode* sNode){
  rootNode->addChild(sNode);
}