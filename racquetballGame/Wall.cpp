#include "Wall.h"
#include "Audio/src/audio.h"

Wall::Wall(Ogre::SceneManager* scnMgr, Simulator* sim, Ogre::Plane p, btQuaternion rot, btVector3 pos){
	// Set our texture for the walls
    std::string material_str = "Examples/BumpyMetal";
    scoreboard = NULL;
	isKinematic = false;
	mass = 0;


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
    if(volume->hitRegistered())
      playSound("Audio/sounds/bounce.wav", SDL_MIX_MAXVOLUME/6);

    if(scoreboard){
        if (volume->hitRegistered()){
            if(!scoreboard->reset){
                scoreboard->rally++;
                playSound("Audio/sounds/score2.wav", SDL_MIX_MAXVOLUME);
                scoreboard->reset = true;
            }
        }
        else {
            scoreboard->reset = false;
        }
    }
}
