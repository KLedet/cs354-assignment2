#include <Ogre.h>
#include <OgreMotionState.h>

class Simulator {
protected:
     btDefaultCollisionConfiguration* collisionConfiguration;
     btCollisionDispatcher* dispatcher;
     btBroadphaseInterface* overlappingPairCache;
     btSequentialImpulseConstraintSolver* solver;
     btDiscreteDynamicsWorld* dynamicsWorld;
     btAlignedObjectArray<btCollisionShape*> collisionShapes;
     std::deque<GameObject*> objList;
public:
     Simulator();
     ~Simulator();
     void addObject(GameObject* o);
     bool removeObject(GameObject* o);
     void stepSimulation(const Ogre::Real elapsedTime,
     int maxSubSteps = 1, const Ogre::Real fixedTimestep = 1.0f/60.0f);
};


class GameObject {
protected:
  Ogre::String name;
  Ogre::SceneManager* sceneMgr;
  Ogre::SceneNode* rootNode;
  Ogre::Entity* geom;
  OgreMotionState* motionState;

  Simulator* simulator;
  btCollisionShape* shape;
  btRigidBody* body;
  btTransform tr;
  btVector3 inertia;

  btScalar mass;
  btScalar restitution;
  btScalar friction;
  bool kinematic;
  bool needsUpdates;

  CollisionContext* context;
  BulletContactCallback* cCallBack;

public:
  void addToSimulator();
  btRigidBody* getBody(){return body;}

};