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
  s_paused = true;
  s_start = false;
  singleplayer = true;
  s_modeSetupComplete = false;
  s_networkSetupComplete = false;
  setupComplete = false;
  mIsServer = false;

  controlID = 0; //might use this, might not. we'll see.
  char response;
  char response2[50];
  
  mNetMan = NULL;
  nethandler= NULL;

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
  
  gui = new GUI();

  SDL_Init(SDL_INIT_AUDIO);
  initAudio();
  // Create the scene
  //can we create scene later? so that we can make the GUI and do all the setup and then switch mode
  //createScene();

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
	
	SDL_Init(SDL_INIT_AUDIO);
	initAudio();

  std::cout << "reposition camera" << std::endl;

	// Reposition camera
	Ogre::Vector3 cam_position = player->getPosition() + Ogre::Vector3(-75, 375, 600);
	mCamera->setPosition(cam_position);
}

void OgreBall::initSinglePlayer(){
  std::cout << "SP create scene" << std::endl;
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

  std::cout << "reposition camera" << std::endl;

  // Reposition camera
  Ogre::Vector3 cam_position = player->getPosition() + Ogre::Vector3(-75, 375, 600);
  mCamera->setPosition(cam_position);
}

void OgreBall::initMultiPlayer(){
  std::cout << "MP create scene" << std::endl;
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

  std::cout << "reposition camera" << std::endl;

  // Reposition camera
  Ogre::Vector3 cam_position = player->getPosition() + Ogre::Vector3(-75, 375, 600);
  mCamera->setPosition(cam_position);
}
//---------------------------------------------------------------------------
bool OgreBall::frameRenderingQueued(const Ogre::FrameEvent& fe)
{
    //game state
    if(mWindow->isClosed())
        return false;

    if(mShutDown)
        return false;

    //setup using GUI flags
      
    if(!s_modeSetupComplete && gui->playerModeSelected){
      std::cout << "mode select + init scene" << std::endl;
      if(gui->isSingleplayer){
        std::cout << "init_singleplayer" << std::endl;
        initSinglePlayer();
        mIsServer = true;
        singleplayer = true;
        setupComplete = true;
        s_networkSetupComplete = true;
      }else{
        std::cout << "init_multiplayer" << std::endl;
        initMultiPlayer();
        singleplayer = false;
      }
      s_modeSetupComplete = true;
    }
    
    if(!s_networkSetupComplete && gui->hostStatusSelected){
      std::cout << "create net manager" << std::endl;
      singleplayer = false;
      createNetManager(gui->isServer);
      setupComplete = true;
    }
    if(!s_networkSetupComplete && gui->hostnameSet ){
      std::cout << "Starting client...\n";
      std::cout << "hostname:  " << gui->hostname << std::endl;
      mNetMan->addNetworkInfo(PROTOCOL_TCP, gui->hostname, 49157);
      mNetMan->startClient();
      mNetMan->acceptConnections();
      s_networkSetupComplete = true;
      setupComplete = true;
      std::cout << "Waiting for client connection.\n";
    }
    if(!setupComplete && s_networkSetupComplete && s_modeSetupComplete){

    }

    //device capture
    mKeyboard->capture();
    mMouse->capture();


    gui->injectTimestamps(fe);
    

    if(setupComplete){
      if(singleplayer){
        worldStepSP(fe);
      }else{
        worldStepMP(fe);
      }
    }
    return true;
}
//---------------------------------------------------------------------------
void OgreBall::worldStepMP(const Ogre::FrameEvent& fe){
  const Ogre::Real elapsedTime = fe.timeSinceLastFrame;
  if(mIsServer){
    //this should be done elsewhere
    //TODO: do not handle directly. defer to appropriate handler.
    while(mNetMan->scanForActivity()){
      if(mNetMan->tcpClientData[0]->updated && !s_paused){
        char evt[10];
        char args[20];
        char key;
        int num = 3;
        int index = 0;
        bool readMoreMessages = true;
        for(int i = 0; i < 256; i++){
          std::cout << (unsigned char)mNetMan->tcpClientData[0]->output[i] << " ";
        }
        std::cout << std::endl;
        while(index < 256){
          btVector3 velDelta = btVector3(0.0,0.0,0.0);
          btVector3 vel = player2->getVelocity();
          strcpy(evt, mNetMan->tcpClientData[0]->output + index);
          //std::cout << "evt: " << evt << " " << index;
          index += strlen(evt)+1; // C C C C C 0
          if (strcmp(evt, "KD") == 0){
            
            strcpy(evt, mNetMan->tcpClientData[0]->output + index); 
            index += 2;
            key = evt[0];
            switch(key){
            case('W'):
              velDelta.setY(num);
              break;
            case('S'):
              velDelta.setY(-num);
              break;
            case('A'):
              velDelta.setX(num);
              break;
            case 'D':
              velDelta.setX(-num);
              break;
            default:
              break;
            }
            player2->input(vel + velDelta);
          }
          if(strcmp(evt, "KU") == 0){
            strcpy(evt, mNetMan->tcpClientData[0]->output + index); 
            index+=2;
            key = evt[0];
            switch(key){
            case('W'):
              velDelta.setY(-num);
              break;
            case('S'):
              velDelta.setY(num);
              break;
            case('A'):
              velDelta.setX(-num);
              break;
            case 'D':
              velDelta.setX(num);
              break;
            default:
              break;
            }
            player2->input(vel + velDelta);
          }
          if(strcmp(evt, "MD") == 0){
            player2->swing();
          }
          if(strcmp(evt, "MU") == 0){
            player2->unswing();
          }
          
        }
      }
    }
  
    if(!s_paused){
      mSim->stepSimulation(elapsedTime);
      //update gui
      if(scoreboard->reset){
        gui->updateScore(scoreboard->rally[1], scoreboard->rally[0] );
        //std::cout << scoreboard->rally[0] << " " << scoreboard->rally[1] << std::endl;
        nethandler->sendScore(scoreboard->rally[0], scoreboard->rally[1]);
        scoreboard->reset = false;
      }
    }
    //message clients
    if(mNetMan->getClients() > 0){
      if(!s_start){
        s_paused = !s_paused;
        s_start = true;
      }
    
      nethandler->sendTransform(ball->getTransform(), 'B');
      nethandler->sendTransform(player->getTransform(), '1');
      nethandler->sendTransform(player2->getTransform(), '2');
    }

    // Have camera follow the paddle
    mCamera->lookAt(player->getPosition());
    mCamera->setPosition(player->getPosition() + Ogre::Vector3(-75, 375, 600));

  } else {

    while(mNetMan->scanForActivity()){
      
      char args[20];
      
      int index = 0;
      bool readMoreMessages = true;
      
      
      while(readMoreMessages){
        char evt[40]; //message buffer
        strcpy(evt, mNetMan->tcpServerData.output + index);
        //std::cout << "evt: " << evt << " " << index;
        index += strlen(evt)+1; // C C C C C 0
        
        if(strcmp(evt, "transform") == 0){
          char objID;
          btTransform tr;
          btTransformData dataIn;
          readMoreMessages = true;
          strcpy(evt, mNetMan->tcpServerData.output + index); 
          index+=2;
          objID = evt[0];
          //std::cout << "transform : " << objID << std::endl;
          memcpy(&dataIn, mNetMan->tcpServerData.output + index, sizeof(dataIn));
          tr.deSerializeFloat(dataIn);
          index+= sizeof(dataIn);
          /*
          std::cout << new_pos << std::endl;
          memcpy(&new_rot, mNetMan->tcpServerData.output + index, sizeof(new_rot));
          index += sizeof(new_rot);
          std::cout << new_rot << std::endl;
          */
          
          switch(objID){
            case('B'):
              ball->setTransform(tr);
              break;
            case('1'):
              player->setTransform(tr);
              break;
            case('2'):
              player2->setTransform(tr);
              break;
            default:
              break;
          }
        
        } else if (strcmp(evt, "score") == 0) {
          int score1;
          int score2;
          memcpy(&score1, mNetMan->tcpServerData.output + index, sizeof(score1));
          index+=sizeof(score1);
          memcpy(&score2, mNetMan->tcpServerData.output + index, sizeof(score2));
          index+=sizeof(score1);
          gui->updateScore(score2, score1);
          readMoreMessages = true;
        } else {
          readMoreMessages = false;
        }
        
      }
      /*
      Ogre::SceneNode* ball_node = ball->getNode();
      nethandler->readTransform(ball_node);
      if(mNetMan->scanForActivity()){
          Ogre::SceneNode* player1_node = player->getNode();
          nethandler->readTransform(player1_node);
      }
      if(mNetMan->scanForActivity()){
          Ogre::SceneNode* player2_node = player2->getNode();
          nethandler->readTransform(player2_node);
      }*/
    }
    // Have camera follow the paddle
    mCamera->lookAt(player2->getPosition());
    mCamera->setPosition(player2->getPosition() + Ogre::Vector3(-75, 375, -600));
  }
  
}
//---------------------------------------------------------------------------

void OgreBall::worldStepSP(const Ogre::FrameEvent& fe){
  const Ogre::Real elapsedTime = fe.timeSinceLastFrame;

    //update player velocity before physics stepsimulation 
    if(!s_paused){
        mSim->stepSimulation(elapsedTime);
        //update gui
        if(scoreboard->reset){
          gui->updateScore(scoreboard->rally[1]);
        }
        mCamera->lookAt(player->getPosition());
      mCamera->setPosition(player->getPosition() + Ogre::Vector3(-75, 375, 600));
      }
  
}
//---------------------------------------------------------------------------

bool OgreBall::keyPressed( const OIS::KeyEvent &arg )
{
  if (arg.key == OIS::KC_ESCAPE) {
    mShutDown = true;
  }

	//Inject input into the GUI
	gui->injectDownInput(arg);
  
  if(nethandler){
    nethandler->injectDownInput(arg);
  }
  if(player && mIsServer){
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
  /*
  if(!mIsServer){
    nethandler->injectDownInput(arg);
  }else {
    if(player){
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
  }*/
  //controller handling

  switch(arg.key){
    case OIS::KC_M:
      toggleAudioMute();
      break;
      case OIS::KC_P:
        if(singleplayer){
          s_paused = !s_paused;
        } else {
          if( player2 && !mIsServer){
            player2->input(btVector3(0,0,0));
          }
        }
    default:
      break;
  }
  return true;
}
//---------------------------------------------------------------------------
bool OgreBall::keyReleased(const OIS::KeyEvent &arg)
{

	gui->injectUpInput(arg);

  if(nethandler){
    nethandler->injectUpInput(arg);
  }
  if(player && mIsServer){
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
  /*
  if(!mIsServer){
    nethandler->injectUpInput(arg);
  }else {
    if(player){
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

} */
  return true;
}


//---------------------------------------------------------------------------

bool OgreBall::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
  gui->injectMouseDownInput(id);

  if(nethandler){
    nethandler->injectMouseDownInput(id);
  }
  if(player && mIsServer){
    player->swing();
  }
  /*
  if(setupComplete){ 
  if(!mIsServer){
    nethandler->injectMouseDownInput(id);
  } else {
    if (player)
    player->swing();
  }
}*/
    
	return true;
}

bool OgreBall::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
	gui->injectMouseUpInput(id);
  if(nethandler){
    nethandler->injectMouseUpInput(id);

  }
  if(player && mIsServer){
    player->unswing();
  }
  /*
  if(setupComplete){
   if(!mIsServer){
    nethandler->injectMouseUpInput(id);
  } else {
    if(player)
    player->unswing();
  }
}
  */
	return true;
}

bool OgreBall::mouseMoved(const OIS::MouseEvent &arg){
	gui->injectMouseMovement(arg);
	return true;
}
//-------------------------------------------------------------------
//---------------------------------------------------------------------------
void OgreBall::createSimulator(void)
{
    mSim = new Simulator();
}
//---------------------------------------------------------------------------
void OgreBall::createNetManager(bool isServer)
{
  mIsServer = isServer;
  mNetMan = new NetManager();
  mNetMan->initNetManager();
  nethandler = new NetHandler(mNetMan);

  nethandler->setIsServer(isServer);

  if(isServer){
    std::cout << "Starting server...\n";
    mNetMan->addNetworkInfo(PROTOCOL_TCP, NULL, 49157);
    mNetMan->startServer();
    mNetMan->acceptConnections();
    s_networkSetupComplete = true;
    std::cout << "Waiting for client connection.\n";
  }
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
