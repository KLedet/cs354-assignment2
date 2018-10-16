#include "Wall.h"

Wall::Wall(Ogre::SceneManager* scnMgr, Simulator* sim, Ogre::Plane p, btQuaternion rot, btVector3 pos){
	// Set our texture for the walls
    std::string material_str = "Examples/BumpyMetal";
    scoreboard = NULL;
	isKinematic = false;
	mass = 0;
	kill = false;

	Ogre::Entity* wall = scnMgr->createEntity("wall");
    wall->setMaterialName(material_str);
    Ogre::SceneNode* node = scnMgr->getRootSceneNode()->createChildSceneNode();
    node->attachObject(wall);
    wall->setCastShadows(true);

    shape = new btBoxShape(btVector3(250.0f, 250.0f, 5.0f));
    tr.setIdentity();
	tr.setRotation(rot);
	tr.setOrigin(pos);
    init(node);
    volume = new KillVolume(sim, tr);
    body->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
    sim->addRigidBody(this);
    needsUpdates = true;
}

Wall::~Wall(void){
}

void Wall::update(Ogre::Real elapsedTime){
    volume->update();
    bool hit = volume->hitRegistered();
    
    if(scoreboard){
        if (volume->hitRegistered()){
            if(!scoreboard->reset){
                scoreboard->rally++;
                scoreboard->reset = true;
            }
        }  
        else {
            scoreboard->reset = false;
        }
    }
    if(kill){
        const btCollisionObject* col = volume->getCollidedObject();
        if(col){
            GameObject* obj = static_cast<GameObject*>(col->getUserPointer());
            if(obj) obj->update(0.0f); //doesn't matter
        }
    }
}
