/*
-----------------------------------------------------------------------------
Filename:    OgreBall.cpp
-----------------------------------------------------------------------------
*/

#include "OgreBall.h"
#include "Room.cpp"





//---------------------------------------------------------------------------
OgreBall::OgreBall(void)
{
}
//---------------------------------------------------------------------------
OgreBall::~OgreBall(void)
{
}
//---------------------------------------------------------------------------
void OgreBall::go(void)
{
#ifdef _DEBUG
#ifndef OGRE_STATIC_LIB
    mResourcesCfg = m_ResourcePath + "resources_d.cfg";
    mPluginsCfg = m_ResourcePath + "plugins_d.cfg";
#else
    mResourcesCfg = "resources_d.cfg";
    mPluginsCfg = "plugins_d.cfg";
#endif
#else
#ifndef OGRE_STATIC_LIB
    mResourcesCfg = m_ResourcePath + "resources.cfg";
    mPluginsCfg = m_ResourcePath + "plugins.cfg";
#else
    mResourcesCfg = "resources.cfg";
    mPluginsCfg = "plugins.cfg";
#endif
#endif

    if (!setup())
        return;

    mRoot->startRendering();

    // Clean up
    destroyScene();
}
//---------------------------------------------------------------------------
bool OgreBall::setup(void)
{
  pause = false;
  singleplayer = false;
  controlID = 0; //might use this, might not. we'll see.
  char response;
  char response2[50];
  //TODO: modify this so that it sets the multiplayer flag first
  
  createNetManager();

  std::cout << "Server? (y/n): ";
  std::cin >> response;

  response == 'y' ? mIsServer = true : mIsServer = false;

  nethandler->setIsServer(mIsServer);

  if(mIsServer){
    std::cout << "Starting server...\n";
    mNetMan->addNetworkInfo(PROTOCOL_TCP, NULL, 49157);
    mNetMan->startServer();
    mNetMan->acceptConnections();
    std::cout << "Waiting for client connection.\n";
    while(!mNetMan->pollForActivity(5000))
      continue;
  }else{
    std::cout << "Starting client...\n";
    std::cout << "Input hostname: \n";
    std::cin >> response2;
    mNetMan->addNetworkInfo(PROTOCOL_TCP, response2, 49157);
    mNetMan->startClient();
  }


  mRoot = new Ogre::Root(mPluginsCfg);

  setupResources();

  bool carryOn = configure();
  if (!carryOn) return false;

  chooseSceneManager();
  createCamera();
  createViewports();

  // Set default mipmap level (NB some APIs ignore this)
  Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

  // Create any resource listeners (for loading screens)
  createResourceListener();
  // Load resources
  loadResources();

  // Create our physics simulator
  createSimulator();
  
  // Create the scene
  //can we create scene later? so that we can make the GUI and do all the setup and then switch mode
  createScene();

  createFrameListener();

  return true;
}
//---------------------------------------------------------------------------

void OgreBall::createScene(void)
{
    // Create a diffuse point light
  Ogre::Light* l1 = mSceneMgr->createLight("MainLight");
  l1->setType(Ogre::Light::LT_POINT);
  l1->setCastShadows(false);
  l1->setPosition(0,200,0);
  l1->setDiffuseColour(Ogre::ColourValue::White);

	// Create a directional light for shadows
	Ogre::Light* l2 = mSceneMgr->createLight("DirectionalLight");
	l2->setType(Ogre::Light::LT_DIRECTIONAL);
  l2->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y);
	l2->setPosition(0,200,0);
	l2->setDiffuseColour(Ogre::ColourValue::White);

  Ogre::Light* l3 = mSceneMgr->createLight("DirectionalLight3");
  l3->setType(Ogre::Light::LT_DIRECTIONAL);
  l3->setDirection(Ogre::Vector3::NEGATIVE_UNIT_X);
  l3->setPosition(200,0,0);
  l3->setDiffuseColour(Ogre::ColourValue::White);

  // Set up shadows
  mSceneMgr->setShadowCasterRenderBackFaces(false);
  mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
  mSceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5));

	// Create Scoreboard
  scoreboard = new Scoreboard();

  // Create Room
	Room* room = new Room(mSceneMgr, mSim, scoreboard);

	// Create Ball
	ball = new Ball();
  ball->addToScene(mSceneMgr);
  ball->addToSim(mSim);

  std::cout<<"create player 1" << std::endl;
  // Create Player
  player = new Player(mSceneMgr, mSim, 0);

  std::cout << "create player 2" << std::endl;
  // Create Player
  player2 = new Player(mSceneMgr, mSim, 1);
  player2->setPosition(btVector3(0.0f, 0.0f, -250.0f));
  player2->setRotation2();

  std::cout << "init gui and audio" << std::endl;

  //should probably be done in setup since this is app related stuff
  // Create GUI
	gui = new GUI();
	SDL_Init(SDL_INIT_AUDIO);
	initAudio();

  std::cout << "reposition camera" << std::endl;

	// Reposition camera
	Ogre::Vector3 cam_position = player->getPosition() + Ogre::Vector3(-75, 375, 600);
	mCamera->setPosition(cam_position);
}



//---------------------------------------------------------------------------
bool OgreBall::frameRenderingQueued(const Ogre::FrameEvent& fe)
{

    if(mWindow->isClosed())
        return false;

    if(mShutDown)
        return false;

      //std::cout << "READ MESSAGES" << std::endl;
      
    if(mIsServer){
        //this should be done elsewhere
        //TODO: do not handle directly. defer to appropriate handler.
        while(mNetMan->scanForActivity()){
          if(mNetMan->tcpClientData[0]->updated){

            char key;
            int num;
            btVector3 vel = player2->getVelocity();
            sscanf(mNetMan->tcpClientData[0]->output, "%c:%d", &key, &num);

            printf("About to switch. key:%c, num:%d\n", key, num);

            switch(key){
            case('W'):
                vel.setY(num);
                
                break;
            case('S'):
                vel.setY(-num);
                
                break;
            case('D'):
                vel.setX(-num);
                
                break;
            case('A'):
                vel.setX(num);
                
                break;
            case('M'):
                num > 0 ? player2->swing() : player2->unswing();
                break;
            }
            player2->input(vel);
          }
        }
    }

    // Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();
    //input handled in device listeners

    //update player velocity before physics stepsimulation

		gui->injectTimestamps(fe);
    const Ogre::Real elapsedTime = fe.timeSinceLastFrame;
    if (!pause){
      if(mIsServer){
        //Server frame

        //step sim
        if(!pause)
          mSim->stepSimulation(elapsedTime);
        
        //update gui
    		if(scoreboard->reset){
    			gui->updateScore(scoreboard->rally[0]);
          std::cout << scoreboard->rally[0] << " " << scoreboard->rally[1] << std::endl;
    		}
        
        //message clients
        if(mNetMan->getClients() > 0){
          Ogre::SceneNode* ball_node = ball->getNode();
          Ogre::SceneNode* player1_node = player->getSceneNode();
          Ogre::SceneNode* player2_node = player2->getSceneNode();
          nethandler->sendTransform(ball_node);
          nethandler->sendTransform(player1_node);
          nethandler->sendTransform(player2_node);
        }

        // Have camera follow the paddle
        mCamera->lookAt(player->getPosition());
        mCamera->setPosition(player->getPosition() + Ogre::Vector3(-75, 375, 600));

      } else {

        while(mNetMan->scanForActivity()){
          Ogre::SceneNode* ball_node = ball->getNode();
          nethandler->readTransform(ball_node);
          if(mNetMan->scanForActivity()){
              Ogre::SceneNode* player1_node = player->getSceneNode();
              nethandler->readTransform(player1_node);
          }
          if(mNetMan->scanForActivity()){
              Ogre::SceneNode* player2_node = player2->getSceneNode();
              nethandler->readTransform(player2_node);
          }
        }
        // Have camera follow the paddle
        mCamera->lookAt(player2->getPosition());
        mCamera->setPosition(player2->getPosition() + Ogre::Vector3(-75, 375, -600));
      }
    }
    return true;
}
//---------------------------------------------------------------------------

bool OgreBall::keyPressed( const OIS::KeyEvent &arg )
{
  if (arg.key == OIS::KC_ESCAPE) {
    mShutDown = true;
  }

	//Inject input into the GUI
	gui->injectDownInput(arg);

  if(!mIsServer){
    nethandler->injectDownInput(arg);
  }else {
    btVector3 vel = player->getVelocity();
    btVector3 velDelta = btVector3(0.0,0.0,0.0);
    switch(arg.key){
    case OIS::KC_W:
      velDelta.setY(3);
      break;
    case OIS::KC_S:
      velDelta.setY(-3);
      break;
    case OIS::KC_A:
      velDelta.setX(-3);
      break;
    case OIS::KC_D:
      velDelta.setX(3);
      break;
    default:
      break;
    }
    player->input(vel + velDelta);
  }

  //controller handling

  switch(arg.key){
    case OIS::KC_M:
      toggleAudioMute();
      break;
    default:
      break;
  }
  return true;
}
//---------------------------------------------------------------------------
bool OgreBall::keyReleased(const OIS::KeyEvent &arg)
{

	gui->injectUpInput(arg);
  if(!mIsServer){
    nethandler->injectUpInput(arg);
  }else {
    btVector3 vel = player->getVelocity();
    btVector3 velDelta = btVector3(0.0,0.0,0.0);
    switch(arg.key){
    case OIS::KC_W:
      velDelta.setY(-3);
      break;
    case OIS::KC_S:
      velDelta.setY(3);
      break;
    case OIS::KC_A:
      velDelta.setX(3);
      break;
    case OIS::KC_D:
      velDelta.setX(-3);
      break;
    default:
      break;
    }
    player->input(vel + velDelta);
  }

  return true;
}


//---------------------------------------------------------------------------

bool OgreBall::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
  gui->injectMouseDownInput(id);
  if(!mIsServer){
    nethandler->injectMouseDownInput(id);
  } else {
    player->swing();
  }
    
	return true;
}

bool OgreBall::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
	gui->injectMouseUpInput(id);
  if(!mIsServer){
    nethandler->injectMouseUpInput(id);
  } else {
    player->unswing();
  }
  
	return true;
}

bool OgreBall::mouseMoved(const OIS::MouseEvent &arg){
	gui->injectMouseMovement(arg);
	return true;
}

//---------------------------------------------------------------------------
void OgreBall::createSimulator(void)
{
    mSim = new Simulator();
}
//---------------------------------------------------------------------------
void OgreBall::createNetManager(void)
{
   mNetMan = new NetManager();
   mNetMan->initNetManager();
   nethandler = new NetHandler(mNetMan);
}
//---------------------------------------------------------------------------

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
	int main(int argc, char *argv[])
#endif
	{
		// Create application object
		OgreBall app;

		try {
			app.go();
		} catch(Ogre::Exception& e)  {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
			std::cerr << "An exception has occurred: " <<
				e.getFullDescription().c_str() << std::endl;
#endif
		}

		return 0;
	}

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
