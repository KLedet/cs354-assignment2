#include "Player.h"
#include "Audio/src/audio.h"

Player::Player(Ogre::SceneManager* scnMgr, Simulator* sim, int pNum){

	Ogre::Entity* paddle = scnMgr->createEntity("cube.mesh");
  paddle->setMaterialName("Examples/OgreLogo");
  paddle->setCastShadows(true);
  rootNode = scnMgr->getRootSceneNode()->createChildSceneNode();
  rootNode->attachObject(paddle);
  rootNode->scale(0.8f,0.4f,0.05f);

  last_time =0;
  mVelocity = btVector3(0,0,0);
  rot_0.setEuler(0.0,0.0,0.0);
  rot_1.setEuler(- SIMD_PI / 2 ,0.0,0.0);
  isSwinging = false;
  swingSpeed = 8.0f;

  isKinematic = true;
  mass = 10.0f;
  shape = NULL;

  tr.setIdentity();
  tr.setRotation(btQuaternion(0,0,0,1.0f));
  tr.setOrigin(btVector3(0, 0, 250));

  collisionWorld = sim->getCollisionWorld();
  needsUpdates = true;
	volume = new KillVolume(sim, tr);
  init(rootNode);

  sim->addAction(this);

  playerNum = pNum;
}

void Player::init(Ogre::SceneNode* node){
  btTransform startTransform = tr;

  btConvexShape* convexShape = new btBoxShape(btVector3(35.0f, 13.0f, 3.0f));
  btConvexShape* convexShape2 = new btBoxShape(btVector3(35.0f, 13.0f, 35.0f));

	volume->getTriggerVolume()->setCollisionShape(convexShape2);
  btPairCachingGhostObject* ghostObject = new btPairCachingGhostObject();
  ghostObject->setWorldTransform(startTransform);
  ghostObject->setCollisionShape( convexShape);
  ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

  motionState = new OgreMotionState(startTransform, node);
  motionState->setWorldTransform(startTransform);

  ghostObject->setUserPointer(this);

  controller = new btKinematicCharacterController(ghostObject, convexShape, btScalar(.5f));
  controller->setUseGhostSweepTest(true);
  controller->setGravity(0.0f);

  context = new CollisionContext();
  context->theObject = this;



}

void Player::input(btVector3 newVelocity){
	mVelocity = newVelocity;
	//TODO: rotate with player's orientation
	controller->setWalkDirection(mVelocity);
}

void Player::update(Ogre::Real elapsedTime){
  //printf("num collisions: %d\n", controller->getGhostObject()->getNumOverlappingObjects());
  btTransform transform = controller->getGhostObject()->getWorldTransform();
  motionState->setWorldTransform(transform); //collision will stop working without this
	// printf("%f | ", elapsedTime);
	btVector3 dist = elapsedTime * 100.0 * mVelocity;
	// printf("dist: %f %f %f\n", dist.x(), dist.y(), dist.z());
	btTransform trans;
	btVector3 origin = transform.getOrigin();
  

  
  if(isSwinging){
    last_time += elapsedTime;
  } else if(last_time > 0.0){
    last_time -= elapsedTime;
  }
  float dt = last_time * swingSpeed;
  if (dt > 1.0) {
    dt = 1.0;
    last_time = 1 / swingSpeed;
    isSwinging = false;
  }
  if (dt < 0.0) {
    dt = 0.0;
    last_time = 0.0;
  }
  btQuaternion rot = rot_0.slerp(rot_1, dt);
  

  //printf("w: %f, x: %f, y: %f, z: %f\n",rot.w(), rot.x(), rot.y(), rot.z());
  trans.setRotation(rot);
  
	trans.setOrigin(transform.getOrigin() + dist);
  controller->getGhostObject()->setWorldTransform(trans);
  controller->setWalkDirection(mVelocity);
	motionState->setWorldTransform(trans);


  volume->update();
  volume->getTriggerVolume()->setWorldTransform(btTransform(btQuaternion(1.0, 1.0, 1.0, 0), origin ));
	if(volume->hitRegistered() && isSwinging){
		playSound("Audio/sounds/bounce.wav", SDL_MIX_MAXVOLUME/6);

    const btCollisionObject* col = volume->getCollidedObject();
    if(col){
        GameObject* obj = static_cast<GameObject*>(col->getUserPointer());
        Ball* ball = static_cast<Ball*>(obj);
        if(ball) ball->impulse(playerNum);
    }
	}


}

void Player::swing(){
  isSwinging = true;  
  //controller->setAngularVelocity(btVector3(0, -10.0, 0));
	playSound("Audio/sounds/whiff.wav", SDL_MIX_MAXVOLUME);
}

void Player::unswing(){
  isSwinging = false;
  //controller->setAngularVelocity(btVector3(0,0,0));
}

void Player::setPosition(btVector3 vec){
  btTransform trans;
  trans.setRotation(controller->getGhostObject()->getWorldTransform().getRotation());
  trans.setOrigin(vec);
  controller->getGhostObject()->setWorldTransform(trans);
}

void Player::setRotation2(){
  // btTransform trans;
  // trans.setRotation(rot);
  // trans.setOrigin(controller->getGhostObject()->getWorldTransform().getOrigin());
  // controller->getGhostObject()->setWorldTransform(trans);
  rot_0.setEuler(SIMD_PI,0.0,0.0);
  rot_1.setEuler(SIMD_PI / 2 ,0.0,0.0);
}