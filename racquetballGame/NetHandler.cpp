#include "NetHandler.h"

NetHandler::NetHandler(NetManager* netManager){
	mNetMan = netManager;
	isServer = false;
	connected = false;
}

NetHandler::~NetHandler(){
	delete mNetMan;
}

void NetHandler::addObject(GameObject* obj){
	objList.push_back(obj);
}
void NetHandler::injectDownInput(const OIS::KeyEvent& arg){
	if(!isServer){
		std::cout<< "Keydown" << std::endl;

		switch(arg.key){
		case OIS::KC_W:
			mNetMan->messageServer(PROTOCOL_TCP, "W:3", 5);
			break;
		case OIS::KC_S:
			mNetMan->messageServer(PROTOCOL_TCP, "S:3", 5);
			break;
		case OIS::KC_A:
			mNetMan->messageServer(PROTOCOL_TCP, "A:3", 5);
			break;
		case OIS::KC_D:
			mNetMan->messageServer(PROTOCOL_TCP, "D:3", 5);
			break;
		default:
			break;
		}
	}
}
void NetHandler::injectUpInput(const OIS::KeyEvent& arg){
	if(!isServer){
		std::cout << "keyup" << std::endl;
		switch(arg.key){
		case OIS::KC_W:
			mNetMan->messageServer(PROTOCOL_TCP, "W:0", 5);
			break;
		case OIS::KC_S:
			mNetMan->messageServer(PROTOCOL_TCP, "S:0", 5);
			break;
		case OIS::KC_A:
			mNetMan->messageServer(PROTOCOL_TCP, "A:0", 5);
			break;
		case OIS::KC_D:
			mNetMan->messageServer(PROTOCOL_TCP, "D:0", 5);
			break;
		default:
			break;
		}
	}
}
void NetHandler::injectMouseDownInput(OIS::MouseButtonID id){
	if(!isServer){
		mNetMan->messageServer(PROTOCOL_TCP, "M:1", 5);
		std::cout << "mousedown" << std::endl;
	}
}
void NetHandler::injectMouseUpInput(OIS::MouseButtonID id){
	if(!isServer){
		mNetMan->messageServer(PROTOCOL_TCP, "M:0", 5);
		std::cout << "mouseup" << std::endl;
	}
}

void NetHandler::sendTransform(Ogre::SceneNode* node){
	Ogre::Vector3 pos = node->getPosition();
	Ogre::Quaternion rot = node->getOrientation();
	int message_length = sizeof(pos) + sizeof(rot);
	char msgBuff[message_length];
    memcpy(msgBuff, &pos, sizeof(pos));
    memcpy(msgBuff + sizeof(pos), &rot, sizeof(rot));
	mNetMan->messageClient(PROTOCOL_TCP, 0, msgBuff, 256);
}

void NetHandler::readTransform(Ogre::SceneNode* node){
	Ogre::Vector3 newPos = Ogre::Vector3(0,0,0);// just in case
	Ogre::Quaternion newRot = Ogre::Quaternion(1.0, 0.0, 0.0, 0.0);
    memcpy(&newPos, mNetMan->tcpServerData.output, sizeof(newPos));
    memcpy(&newRot, mNetMan->tcpServerData.output + sizeof(newPos), sizeof(newRot));
    node->setPosition(newPos);
    node->setOrientation(newRot);
}

void NetHandler::messagePump(){
	bool activity = mNetMan->scanForActivity();
	while(activity){
	}
}