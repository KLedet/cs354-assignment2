/*
-----------------------------------------------------------------------------
Filename:    OgreBall.cpp
-----------------------------------------------------------------------------
*/

#include "OgreBall.h"
#include "Room.cpp"
#include "GUI.h"
#include "Audio/src/audio.h"


Ball* ball;
GUI* gui;
btVector3 vel = btVector3(0,0,0);

//---------------------------------------------------------------------------
OgreBall::OgreBall(void)
{
}
//---------------------------------------------------------------------------
OgreBall::~OgreBall(void)
{
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

	// Create Scoreboard
    scoreboard = new Scoreboard();

    // Create Room
	Room* room = new Room(mSceneMgr, mSim, scoreboard);

	// Create Ball
	ball = new Ball(mSceneMgr, mSim);

	// Create Player
    player = new Player(mSceneMgr, mSim);

    // Create Player
    player2 = new Player(mSceneMgr, mSim);
    player2->setPosition(btVector3(0.0f, 0.0f, -150.0f));
    player2->setRotation2();

    // Create GUI
	gui = new GUI();
	SDL_Init(SDL_INIT_AUDIO);
	initAudio();


	// Reposition camera
	Ogre::Vector3 cam_position = player->getPosition() + Ogre::Vector3(-75, 375, 600);
	mCamera->setPosition(cam_position);


    // Set up shadows
    mSceneMgr->setShadowCasterRenderBackFaces(false);
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
    mSceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5));
}



//---------------------------------------------------------------------------
bool OgreBall::frameRenderingQueued(const Ogre::FrameEvent& fe)
{
	// The game loop

    if(mWindow->isClosed())
        return false;

    if(mShutDown)
        return false;

    if(mIsServer){
        bool activity = mNetMan->pollForActivity((int)fe.timeSinceLastFrame);
        //activity ? std::cout << "Activity detected\n" : std::cout << "Activity not detected\n";
        if(activity){
            if(mNetMan->tcpClientData[0]->updated){
                std::cout << "Message Recieved\n";
                char key;
                int num;
                btVector3 vel = btVector3(0,0,0);
                sscanf(mNetMan->tcpClientData[0]->output, "%c:%d", &key, &num);

                printf("About to switch. key:%c, num:%d\n", key, num);

                switch(key){
                case('W'):
                    vel.setY(num);
                    player2->input(vel);
                    break;
                case('S'):
                    vel.setY(-num);
                    player2->input(vel);
                    break;
                case('D'):
                    vel.setX(-num);
                    player2->input(vel);
                    break;
                case('A'):
                    vel.setX(num);
                    player2->input(vel);
                    break;
                case('M'):
                    num > 0 ? player2->swing() : player2->unswing();
                    break;
                }
            }

        }
    }

    // Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();
    //input handled in device listeners


    // Have camera follow the paddle
    mCamera->lookAt(player->getPosition());
    mCamera->setPosition(player->getPosition() + Ogre::Vector3(-75, 375, 600));

    //update player velocity before physics stepsimulation

		gui->injectTimestamps(fe);
    const Ogre::Real elapsedTime = fe.timeSinceLastFrame;

    mSim->stepSimulation(elapsedTime);

    if(ball->getBody()->getLinearVelocity().norm() < 10){
        scoreboard->rally = 0;
        scoreboard->reset = true;
        ball->update(0);
    }

		if(scoreboard->reset){
			gui->updateScore(scoreboard->rally);
		}
    // printf("rally: %d\n", scoreboard->rally);

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
