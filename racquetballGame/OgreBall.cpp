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

	// Set ambient light
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

	// Create a diffuse light
	Ogre::Light* l = mSceneMgr->createLight("MainLight");
	l->setType(Ogre::Light::LT_POINT);
	l->setPosition(0,500,50);
	l->setDiffuseColour(Ogre::ColourValue::White);

	// Add skybox
	// mSceneMgr->setSkyBox(true, "Examples/MorningSkyBox", 5000, false);

	// Create Room
  scoreboard = new Scoreboard();

	Room* room = new Room(mSceneMgr, mSim, scoreboard);

	// Create Ball
	ball = new Ball(mSceneMgr, mSim);
	 //TODO: add way to specify initial position and rotation in constructor
	// Create GUI
  player = new Player(mSceneMgr, mSim);

	gui = new GUI();
	SDL_Init(SDL_INIT_AUDIO);
	initAudio();

	// Reposition camera
	Ogre::Vector3 cam_position = Ogre::Vector3(0, 300,  750);
	mCamera->setPosition(cam_position);
}



//---------------------------------------------------------------------------
bool OgreBall::frameRenderingQueued(const Ogre::FrameEvent& fe)
{
	//pretty much the game loop

    if(mWindow->isClosed())
        return false;

    if(mShutDown)
        return false;

    // Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();
    //input handled in device listeners


    // Just have this for now so we can tell this function is being called
    // repeatedly
    mCamera->lookAt(player->getPosition());

    //update player velocity before physics stepsimulation

    /*do physics step here?
		calculate timestep
		pass into Sim.stepsim
	*/

		gui->injectTimestamps(fe);
    const Ogre::Real elapsedTime = fe.timeSinceLastFrame;

    mSim->stepSimulation(elapsedTime);

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

  btVector3 vel = btVector3(0,0,0); //TODO: might need to be a btVector3 not sure
  switch(arg.key){
  	case OIS::KC_W:
  		vel.setY(1.0);
  		break;
  	case OIS::KC_S:
  		vel.setY(-1.0);
  		break;
  	case OIS::KC_A:
  		vel.setX(-1.0);
  		break;
  	case OIS::KC_D:
  		vel.setX(1.0);
  		break;
		case OIS::KC_M:
			toggleAudioMute();
  	default:
  		break;
  }
  if(vel != btVector3(0,0,0))
    {
      // printf("%f %f %f\n", vel.x(), vel.y(), vel.z());
      player->input(vel);
    }
  return true;
}
//---------------------------------------------------------------------------
bool OgreBall::keyReleased(const OIS::KeyEvent &arg)
{

	gui->injectUpInput(arg);
	btVector3 vel = btVector3(0,0,0);

  switch(arg.key){
  	case OIS::KC_W:
  		vel.setY(0);
        player->input(vel);
  		break;
  	case OIS::KC_S:
  		vel.setY(0);
        player->input(vel);
  		break;
  	case OIS::KC_A:
  		vel.setX(0);
        player->input(vel);
  		break;
  	case OIS::KC_D:
  		vel.setX(0);
        player->input(vel);
  		break;
  	default:
  		break;
  }
   //TODO:: add a player* to the app
    return true;
}


//---------------------------------------------------------------------------

bool OgreBall::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
	gui->injectMouseDownInput(id);
    player->swing();
	return true;
}

bool OgreBall::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
	gui->injectMouseUpInput(id);
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
