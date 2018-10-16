#include "Player.h"
#include "Audio/src/audio.h"

Player::Player(Ogre::SceneManager* scnMgr, Simulator* sim){

	Ogre::Entity* paddle = scnMgr->createEntity("cube.mesh");
  paddle->setMaterialName("Examples/OgreLogo");
  paddle->setCastShadows(true);
  rootNode = scnMgr->getRootSceneNode()->createChildSceneNode("Cubo");
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
  tr.setOrigin(btVector3(0, 0, 150));

  collisionWorld = sim->getCollisionWorld();
  needsUpdates = true;
	volume = new KillVolume(sim, tr);
  init(rootNode);

  sim->addAction(this);
}

void Player::init(Ogre::SceneNode* node){
  btTransform startTransform = tr;

  btConvexShape* convexShape = new btBoxShape(btVector3(35.0f, 13.0f, 3.0f));
	volume->getTriggerVolume()->setCollisionShape(convexShape);
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
  }
  if (dt < 0.0) {
    dt = 0.0;
    last_time = 0.0;
  }
  btQuaternion rot = rot_0.slerp(rot_1, dt);
  

  printf("w: %f, x: %f, y: %f, z: %f\n",rot.w(), rot.x(), rot.y(), rot.z());
  trans.setRotation(rot);
  //printf("w: %f, x: %f, y: %f, z: %f\n",rot3.w(), rot3.x(), rot3.y(), rot3.z());
  
	trans.setOrigin(transform.getOrigin() + dist);
  controller->getGhostObject()->setWorldTransform(trans);
  controller->setWalkDirection(mVelocity);
	motionState->setWorldTransform(trans);

	if(volume->hitRegistered()){
		playSound("Audio/sounds/bounce.wav", SDL_MIX_MAXVOLUME/6);
	}


}

void Player::swing(){
  isSwinging = true;  
  //controller->setAngularVelocity(btVector3(0, -10.0, 0));
	playSound("Audio/sounds/whiff.wav", SDL_MIX_MAXVOLUME);
}

void Player::unswing(){
  last_time = 1 / swingSpeed;
  isSwinging = false;
  //controller->setAngularVelocity(btVector3(0,0,0));
}
