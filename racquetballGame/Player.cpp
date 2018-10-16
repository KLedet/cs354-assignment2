#include "Player.h"
#include "Audio/src/audio.h"

Player::Player(Ogre::SceneManager* scnMgr, Simulator* sim){

	Ogre::Entity* paddle = scnMgr->createEntity("cube.mesh");
  paddle->setMaterialName("Examples/OgreLogo");
  paddle->setCastShadows(true);
  rootNode = scnMgr->getRootSceneNode()->createChildSceneNode("Cubo");
  rootNode->attachObject(paddle);
  rootNode->scale(0.8f,0.4f,0.05f);

  mVelocity = btVector3(0,0,0);
  isKinematic = true;
  mass = 10.0f;
  shape = NULL;

  tr.setIdentity();
  tr.setRotation(btQuaternion(0,0,0,1.0f));
  tr.setOrigin(btVector3(0, 0, 150));

  collisionWorld = sim->getCollisionWorld();
  needsUpdates = true;

  init(rootNode);
  

  sim->addAction(this);
}

void Player::init(Ogre::SceneNode* node){
  btTransform startTransform = tr;
  
  btConvexShape* convexShape = new btBoxShape(btVector3(35.0f, 13.0f, 3.0f));
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

  trans.setRotation(trans.getRotation() + rotation*elapsedTime*50);

  btQuaternion rot = trans.getRotation();
  //printf("w: %f, x: %f, y: %f, z: %f\n",rot.w(), rot.x(), rot.y(), rot.z());

	trans.setOrigin(transform.getOrigin() + dist);
  controller->getGhostObject()->setWorldTransform(trans);
  controller->setWalkDirection(mVelocity);
	motionState->setWorldTransform(trans);
}

void Player::swing(){
  btScalar yaw = 80.0f;
  btScalar pitch = 0.0f;
  btScalar roll = 0.0f;
  printf("Swinging\n");

  rotation = btQuaternion(80.0f, 0.0f, 0.0f);
  //controller->setAngularVelocity(btVector3(0, -10.0, 0));
	playSound("Audio/sounds/whiff.wav", SDL_MIX_MAXVOLUME);
}

void Player::unswing(){
  btScalar yaw = 0.0f;
  btScalar pitch = 0.0f;
  btScalar roll = 0.0f;
  printf("Un-Swinging\n");

  rotation = btQuaternion(0.0f, 0.0f, 0.0f);
  //controller->setAngularVelocity(btVector3(0,0,0));
}
