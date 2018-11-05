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
#include "NetHandler.h"
//---------------------------------------------------------------------------


class OgreBall : public BaseApplication
{

public:
    //this class should have not have any game objects
    //TODO: come up with way of updating these dynamically
    //and then get rid of them
	Player* player;
    Player* player2;
	Ball* ball;
    
    //this may be fine
    Scoreboard* scoreboard;

    //Hooks for handlers
    GUI* gui;
    NetHandler* nethandler;
    OgreBall(void);
    virtual ~OgreBall(void);

    virtual void go(void);
protected:
    //setup flags
    bool                        singleplayer;
    bool                        mIsServer;

    //state flags
    bool                        pause;

    int controlID;
    Simulator*                  mSim;
    NetManager*                 mNetMan;

    virtual void createScene(void);
    virtual bool setup();
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);
    virtual void createSimulator(void);
    virtual void createNetManager(void);
    bool keyPressed( const OIS::KeyEvent &arg );
    bool keyReleased(const OIS::KeyEvent &arg);
	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	bool mouseMoved(const OIS::MouseEvent &arg);

};

//---------------------------------------------------------------------------

#endif // #ifndef __OgreBall_h_

//---------------------------------------------------------------------------
