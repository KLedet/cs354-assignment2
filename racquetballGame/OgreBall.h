/*
-----------------------------------------------------------------------------
Filename:    OgreBall.h
-----------------------------------------------------------------------------
*/

#ifndef __OgreBall_h_
#define __OgreBall_h_

#include "BaseApplication.h"
#include "Player.h"
#include "Ball.h"
#include "Volume.h"
#include "GUI.h"
#include "Audio/src/audio.h"
//---------------------------------------------------------------------------


class OgreBall : public BaseApplication
{


public:
	Player* player;
  Player* player2;
	Ball* ball;
    Scoreboard* scoreboard;
    GUI* gui;
    btVector3 vel = btVector3(0,0,0);

    OgreBall(void);
    virtual ~OgreBall(void);

protected:
    virtual void createScene(void);
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);
    bool keyPressed( const OIS::KeyEvent &arg );
    bool keyReleased(const OIS::KeyEvent &arg);
		bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
		bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
		bool mouseMoved(const OIS::MouseEvent &arg);

};

//---------------------------------------------------------------------------

#endif // #ifndef __OgreBall_h_

//---------------------------------------------------------------------------
