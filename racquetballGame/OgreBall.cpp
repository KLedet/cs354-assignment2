/*
-----------------------------------------------------------------------------
Filename:    OgreBall.cpp
-----------------------------------------------------------------------------
*/

#include "OgreBall.h"
#include "Room.cpp"
#include "Ball.cpp"

Ball* ball;

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
    l->setPosition(20,80,50);
    l->setDiffuseColour(Ogre::ColourValue::White);

    // Add skybox
    mSceneMgr->setSkyBox(true, "Examples/MorningSkyBox", 5000, false);

    // Create Room
    Room* room = new Room(mSceneMgr); 

    // Create Ball
    ball = new Ball(mSceneMgr);

    // Reposition camera
    Ogre::Vector3 cam_position = Ogre::Vector3(ball->getPosition().x - 100, ball->getPosition().y, ball->getPosition().z);
    mCamera->setPosition(cam_position);
}

//---------------------------------------------------------------------------
bool OgreBall::frameRenderingQueued(const Ogre::FrameEvent& fe)
{
  bool ret = BaseApplication::frameRenderingQueued(fe);

  if(!processUnbufferedInput(fe))
    return false;

    // Move our ball, and have camera follow the ball
    ball->move(fe);
    mCamera->lookAt(ball->getPosition());

  return ret;
}
//---------------------------------------------------------------------------

bool OgreBall::processUnbufferedInput(const Ogre::FrameEvent& fe)
{
    // Set up variables for camera and light
    static bool mouseDownLastFrame = false;
    static Ogre::Real toggleTimer = 0.0;
    static Ogre::Real rotate = .13;
    static Ogre::Real move = 250;
    Ogre::Real speed = 100;
    Ogre::Vector3 dir = mCamera->getDirection();
    dir.normalise();

    // See if mouse has been pressed down
    bool leftMouseDown = mMouse->getMouseState().buttonDown(OIS::MB_Left);

    // Camera controls
    if(mKeyboard->isKeyDown(OIS::KC_W))
        mCamera->move( dir * speed * fe.timeSinceLastFrame );

    if(mKeyboard->isKeyDown(OIS::KC_S))
        mCamera->move( -1 * dir * speed * fe.timeSinceLastFrame );

    // Turn light on and off
    if (leftMouseDown && !mouseDownLastFrame)
    {
      Ogre::Light* light = mSceneMgr->getLight("MainLight");
      light->setVisible(!light->isVisible());
    }

    mouseDownLastFrame = leftMouseDown;

    return true;
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
