#include "Wall.h"
#include "Audio/src/audio.h"

Wall::Wall(btQuaternion rot, btVector3 origin, bool backwall){
    isBackwall = backwall;
	// Set our texture for the walls
    
    scoreboard = NULL;
	init(origin, rot);

	kill = false;
    isActive = false;

    volume = new Volume(tr);
    
    lastHit=false;
    needsUpdates = true;

}

Wall::~Wall(void){
}

void Wall::init(btVector3 origin, btQuaternion rot){
    tr.setIdentity();
    tr.setRotation(rot);
    tr.setOrigin(origin);
}

void Wall::addToSim(Simulator *mSim){
    if (!shape)
        shape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));

    btVector3 inertia = btVector3(0,0,0);
    btScalar restitution = .9;
    btScalar friction = 0.0;
    btScalar mass = 0;

    motionState = new OgreMotionState(tr, rootNode);
    motionState->setWorldTransform(tr);

    if (mass != 0.0f) 
        shape->calculateLocalInertia(mass, inertia);

    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, inertia);
    rbInfo.m_restitution = restitution;
    rbInfo.m_friction = friction;

    body = new btRigidBody(rbInfo);
    body->setUserPointer(this);
    body->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
    
    mSim->addRigidBody(this);

    volume->addToSim(mSim);
    //add volume to sim too
}

void Wall::addToScene(Ogre::SceneManager *scnMgr){
    Ogre::Entity* wall;
    std::string material_str = "Examples/BumpyMetal";

    if (isBackwall){
       wall = scnMgr->createEntity("backwall")  ;
        shape = new btBoxShape(btVector3(250.0f, 250.f, 5.0f));
    } else {
        wall = scnMgr->createEntity("wall");
        shape = new btBoxShape(btVector3(250.0f, 500.0f, 5.0f));
    }
    wall->setMaterialName(material_str);
    rootNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    rootNode->attachObject(wall);
    wall->setCastShadows(true);
}

void Wall::update(Ogre::Real elapsedTime){
    volume->update();
    bool hit = volume->hitRegistered();
    
    bool onEntered = hit && !lastHit;
    lastHit = hit;
    if(onEntered){
        playSound("Audio/sounds/bounce.wav", SDL_MIX_MAXVOLUME/6);
        if(kill){
            const btCollisionObject* col = volume->getCollidedObject();
            if(col){
                GameObject* obj = static_cast<GameObject*>(col->getUserPointer());
                if(obj) obj->update((Ogre::Real)this->getID()); //doesn't matter
            }
        }
        if(scoreboard){
            scoreboard->rally[id]++;
            scoreboard->reset = true;
            playSound("Audio/sounds/score2.wav", SDL_MIX_MAXVOLUME);
        }
    }
}
