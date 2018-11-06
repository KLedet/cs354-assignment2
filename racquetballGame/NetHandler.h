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
#include "GameObject.h"
//wrapper to handle OIS events with NetManager

class NetHandler;
struct NetEvent{
public:
	enum EventType {
		NET_TRANSFORM,
		NET_KEY_UP,
		NET_KEY_DOWN,
		NET_MOUSE_UP,
		NET_MOUSE_DOWN,
		NET_CLOSE,
		NET_AUDIO,
	};

	NetEvent(NetHandler* handler, NetEvent::EventType type, char* args);
	~NetEvent(void);
};
class InputHandler{
public:
	InputHandler(void) {}
	~InputHandler(void) {}
	virtual void injectDownInput(const OIS::KeyEvent& arg){}
	virtual void injectUpInput(const OIS::KeyEvent& arg){}
	virtual void injectMouseDownInput(OIS::MouseButtonID id){}
	virtual void injectMouseUpInput(OIS::MouseButtonID id){}
};
class NetHandler : public InputHandler{
protected:
	
	NetManager* mNetMan;
	bool isServer;
	bool connected;
	std::vector<GameObject*> objList;
public:
	NetHandler(NetManager* netMan);
	~NetHandler(void);

	void addObject(GameObject *obj);
	virtual void injectDownInput(const OIS::KeyEvent& arg);
	virtual void injectUpInput(const OIS::KeyEvent& arg);
	virtual void injectMouseDownInput(OIS::MouseButtonID id);
	virtual void injectMouseUpInput(OIS::MouseButtonID id);
	bool getIsServer(){return isServer;}
	void setIsServer(bool server_val){ isServer = server_val;}
	bool connectionEstablished(){return connected;}
	void sendTransform(Ogre::SceneNode* node);
	void readTransform(Ogre::SceneNode* node);
	void messagePump();
};
#endif