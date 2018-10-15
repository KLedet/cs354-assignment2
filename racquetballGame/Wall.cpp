#include "Wall.h"

Wall::Wall(Ogre::SceneManager* scnMgr, Simulator* sim, Ogre::Plane p, btQuaternion rot, btVector3 pos){
	// Set our texture for the walls
    std::string material_str = "Examples/BumpyMetal";

	isKinematic = false;
	mass = 0;
	

	Ogre::Entity* wall = scnMgr->createEntity("wall");
    wall->setMaterialName(material_str);
    Ogre::SceneNode* node = scnMgr->getRootSceneNode()->createChildSceneNode();
    node->attachObject(wall);
    wall->setCastShadows(true);

    shape = new btBoxShape(btVector3(251.0f, 251.0f, 10.0f));
    tr.setIdentity();
	tr.setRotation(rot);
	tr.setOrigin(pos);
    init(node);
    body->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
    sim->addObject(this);
    printf("WALL HAS COLLISION RESPONSE: ");
    printf(body->hasContactResponse() ? "true\n" : "false\n");
    needsUpdates = false;
}

Wall::~Wall(void){
}

