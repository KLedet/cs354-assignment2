#ifndef __NetHandler_h_
#define __NetHandler_h_

#include <Ogre.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#  include <OIS/OISEvents.h>
#  include <OIS/OISInputManager.h>
#  include <OIS/OISKeyboard.h>
#  include <OIS/OISMouse.h>

#else
#  include <OISEvents.h>
#  include <OISInputManager.h>
#  include <OISKeyboard.h>
#  include <OISMouse.h>

#endif

#include "NetManager.h"

//wrapper to handle OIS events with NetManager
class NetHandler{
protected:
	enum {
		MSG_TRANSFORM,
		MSG_KEY_UP,
		MSG_KEY_DOWN,
		MSG_MOUSE_UP,
		MSG_MOUSE_DOWN,
	};
	NetManager* mNetMan;
	bool server;
	bool connected;
public:
	NetHandler(NetManager* netMan);
	~NetHandler(void);
	void injectDownInput(const OIS::KeyEvent& arg);
	void injectUpInput(const OIS::KeyEvent& arg);
	void injectMouseDownInput(OIS::MouseButtonID id);
	void injectMouseUpInput(OIS::MouseButtonID id);
	bool isServer(){return server;}
	void setIsServer(bool server_val){ server = server_val;}
	bool connectionEstablished(){return connected;}
	void sendTransform(Ogre::SceneNode* node);
	void readTransform(Ogre::SceneNode* node);
	void messagePump();
};

#endif