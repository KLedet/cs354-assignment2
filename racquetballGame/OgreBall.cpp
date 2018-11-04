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

bool OgreBall::setup(void)
{
    char response;
    char response2[50];

    createNetManager();

    std::cout << "Server? (y/n): ";
    std::cin >> response;

    response == 'y' ? mIsServer = true : mIsServer = false;

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
    createScene();

    if(mIsServer){
        while(true){
            if(strcmp(mNetMan->tcpClientData[0]->output, "Ready") == 0){
                mNetMan->messageClient(PROTOCOL_TCP, 0, "Ready", 6);
                break;
            }
            mNetMan->pollForActivity(5000);
        }
    }else{
        mNetMan->messageServer(PROTOCOL_TCP, "Ready", 6);
        while(true){
            mNetMan->pollForActivity(10000);
            if(strcmp(mNetMan->tcpServerData.output, "Ready") == 0);
                break;
        }
    }

    createFrameListener();

    return true;
}
//---------------------------------------------------------------------------

void OgreBall::createScene(void)
{
  vel = btVector3(0.,0.,0.);
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
  player = new Player(mSceneMgr, mSim, 1);

  std::cout << "create player 2" << std::endl;
  // Create Player
  player2 = new Player(mSceneMgr, mSim, 2);
  player2->setPosition(btVector3(0.0f, 0.0f, -250.0f));
  player2->setRotation2();

  std::cout << "init gui and audio" << std::endl;

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
	// The game loop
  //no logic should be written here. it should all be done within the objects themselves
    
    if(mWindow->isClosed())
        return false;

    if(mShutDown)
        return false;

    if(mIsServer){
        //this should be done elsewhere
        bool activity = mNetMan->pollForActivity((int)fe.timeSinceLastFrame);
        //activity ? std::cout << "Activity detected\n" : std::cout << "Activity not detected\n";
        if(activity){
            if(mNetMan->tcpClientData[0]->updated){
                //std::cout << "Message Recieved\n";
                char key;
                int num;
                btVector3 vel = player2->getVelocity();
                sscanf(mNetMan->tcpClientData[0]->output, "%c:%d", &key, &num);

                // printf("About to switch. key:%c, num:%d\n", key, num);

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
        // Have camera follow the paddle
        mCamera->lookAt(player->getPosition());
        mCamera->setPosition(player->getPosition() + Ogre::Vector3(-75, 375, 600));
    }
    else{
      bool activity = mNetMan->pollForActivity((int)fe.timeSinceLastFrame);
      if(activity){
        if(mNetMan->tcpServerData.updated){
            //std::cout << "Message Recieved\n";
            char key;

            // Maybe need to adjust this

            Ogre::Real bx, by, bz, playerPosX, playerPosY, playerPosZ, playerRotW, playerRotX, playerRoty, playerRotZ, player2PosX, player2PosY, player2PosZ, player2RotW, player2RotX, player2Roty, player2RotZ;
            sscanf(mNetMan->tcpServerData.output, "B:%f,%f,%f,P1:{(P:%f,%f,%f),(R:%f,%f,%f,%f)},P2:{(P:%f,%f,%f),(R:%f,%f,%f,%f)}",
             &bx, &by, &bz, &playerPosX, &playerPosY, &playerPosZ, &playerRotW, &playerRotX, &playerRoty, &playerRotZ, &player2PosX, &player2PosY, &player2PosZ, &player2RotW, &player2RotX, &player2Roty, &player2RotZ);

            // printf("About to switch. key:%c, num:%d\n", key, num);

            Ogre::Vector3 vec = Ogre::Vector3(bx,by,bz);
            ball->setPosition(vec);

            vec = Ogre::Vector3(playerPosX, playerPosY, playerPosZ);
            Ogre::Quaternion rot = Ogre::Quaternion(playerRotW, playerRotX, playerRoty, playerRotZ);
            player->setOgrePosition(vec);
            player->setOgreRotation(rot);

            vec = Ogre::Vector3(player2PosX, player2PosY, player2PosZ);
            rot = Ogre::Quaternion(player2RotW, player2RotX, player2Roty, player2RotZ);
            player2->setOgrePosition(vec);
            player2->setOgreRotation(rot);
        }

    }
    // Have camera follow the paddle
    mCamera->lookAt(player2->getPosition());
    mCamera->setPosition(player2->getPosition() + Ogre::Vector3(-75, 375, -600));
    }

    // Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();
    //input handled in device listeners

    //update player velocity before physics stepsimulation

		gui->injectTimestamps(fe);
    const Ogre::Real elapsedTime = fe.timeSinceLastFrame;
    
    if(mIsServer){
      mSim->stepSimulation(elapsedTime);

      if(ball->getBody()->getLinearVelocity().norm() < 10){
          scoreboard->rally[0] = 0;
          scoreboard->reset = true;
          ball->update(0);
      }
      
  		if(scoreboard->reset){
  			gui->updateScore(scoreboard->rally[0]);
  		}
      
      if(mNetMan->getClients() > 0){
        char clientBuff[128];
        snprintf(clientBuff, sizeof(clientBuff), "B:%.2f,%.2f,%.2f,P1:{(P:%.2f,%.2f,%.2f),(R:%.2f,%.2f,%.2f,%.2f)},P2:{(P:%.2f,%.2f,%.2f),(R:%.2f,%.2f,%.2f,%.2f)}",
         ball->getPosition().x, ball->getPosition().y, ball->getPosition().z,
         player->getPosition().x, player->getPosition().y, player->getPosition().z,
         player->getOrientation().w, player->getOrientation().x, player->getOrientation().y, player->getOrientation().z,
         player2->getPosition().x, player2->getPosition().y, player2->getPosition().z,
         player2->getOrientation().w, player2->getOrientation().x, player2->getOrientation().y, player2->getOrientation().z);
        mNetMan->messageClient(PROTOCOL_TCP, 0, clientBuff, 128);
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

  //directly setting velocities and hardcoding messages
  //should instead do:
  //InputHandler->injectUpInput
  //NetHandler->injectUpInput
  //that way we neither A) have to register listeners for everything that needs to handle a key event nor
  // B) hard code anything

  switch(arg.key){
  	case OIS::KC_W:
        if(!mIsServer){
            mNetMan->messageServer(PROTOCOL_TCP, "W:3", 5);
        }
        else
  		    vel.setY(3.0);
  		break;
  	case OIS::KC_S:
        if(!mIsServer){
            mNetMan->messageServer(PROTOCOL_TCP, "S:3", 5);
        }
        else
  		    vel.setY(-3.0);
  		break;
  	case OIS::KC_A:
        if(!mIsServer){
            mNetMan->messageServer(PROTOCOL_TCP, "A:3", 5);
        }
        else
  		    vel.setX(-3.0);
  		break;
  	case OIS::KC_D:
        if(!mIsServer){
            mNetMan->messageServer(PROTOCOL_TCP, "D:3", 5);
        }
        else
  		    vel.setX(3.0);
  		break;
	case OIS::KC_M:
		toggleAudioMute();
  	default:
  		break;
  }
  if(vel != btVector3(0,0,0))
    {
      player->input(vel);
    }
  return true;
}
//---------------------------------------------------------------------------
bool OgreBall::keyReleased(const OIS::KeyEvent &arg)
{

	gui->injectUpInput(arg);

  

  switch(arg.key){
  	case OIS::KC_W:
        vel.setY(0);
        if(!mIsServer){
            mNetMan->messageServer(PROTOCOL_TCP, "W:0", 5);
        }
        else
            player->input(vel);
  		break;
  	case OIS::KC_S:
  		vel.setY(0);
        if(!mIsServer){
            mNetMan->messageServer(PROTOCOL_TCP, "S:0", 5);
        }
        else
            player->input(vel);
  		break;
  	case OIS::KC_A:
  		vel.setX(0);
        if(!mIsServer){
            mNetMan->messageServer(PROTOCOL_TCP, "A:0", 5);
        }
        else
            player->input(vel);
  		break;
  	case OIS::KC_D:
  		vel.setX(0);
        if(!mIsServer){
            mNetMan->messageServer(PROTOCOL_TCP, "D:0", 5);
        }
        else
            player->input(vel);
  		break;
  	default:
  		break;
  }
    return true;
}


//---------------------------------------------------------------------------

bool OgreBall::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
    gui->injectMouseDownInput(id);
    if(!mIsServer)
            mNetMan->messageServer(PROTOCOL_TCP, "M:1", 5);
    else
        player->swing();
	return true;
}

bool OgreBall::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
	gui->injectMouseUpInput(id);
    if(!mIsServer)
            mNetMan->messageServer(PROTOCOL_TCP, "M:0", 5);
    else
        player->unswing();
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
