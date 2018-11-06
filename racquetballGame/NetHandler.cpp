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
		char msgBuff[] = "keydown";
		char keyBuff[2];
		char key;
		int num;
		switch(arg.key){
		case OIS::KC_W:
			key = 'W';
			num = 3;
			break;
		case OIS::KC_S:
			key = 'S';
			break;
		case OIS::KC_A:
			key = 'A';
			break;
		case OIS::KC_D:
			key = 'D';
			break;
		default:
			return; //return early to avoid messaging anything
			break;
		}
		keyBuff[0] = key;
		keyBuff[1] = 0;
		//sprintf(msgBuff, "%c:%d", key, num);
		mNetMan->messageServer(PROTOCOL_TCP, msgBuff, 8);
		mNetMan->messageServer(PROTOCOL_TCP, keyBuff, 2);
	}
}
void NetHandler::injectUpInput(const OIS::KeyEvent& arg){
	if(!isServer){
		std::cout << "keyup" << std::endl;
		char msgBuff[] = "keyup";
		char keyBuff[2];
		int num;
		char key;
		switch(arg.key){
		case OIS::KC_W:
			key = 'W';
			num = -3;
			
			break;
		case OIS::KC_S:
			key = 'S';
			num = 3;
			
			break;
		case OIS::KC_A:
			key = 'A';
			num = -3;
			
			break;
		case OIS::KC_D:
			key = 'D';
			num = 3;
			break;
		default:
			return; //return early to avoid messaging anything
			break;
		}
		keyBuff[0] = key;
		keyBuff[1] = 0;
		//sprintf(msgBuff, "%c:%d", key, num);
		mNetMan->messageServer(PROTOCOL_TCP, msgBuff, 6);
		mNetMan->messageServer(PROTOCOL_TCP, keyBuff, 2);
		//mNetMan->messageServer(PROTOCOL_TCP, numBuff, sizeof(numBuff)+1);
	}
}
void NetHandler::injectMouseDownInput(OIS::MouseButtonID id){
	if(!isServer){
		char msgBuff[] = "mousedown";
		mNetMan->messageServer(PROTOCOL_TCP, msgBuff, 10);
		std::cout << "mousedown" << std::endl;
	}
}
void NetHandler::injectMouseUpInput(OIS::MouseButtonID id){
	if(!isServer){
		char msgBuff[] = "mouseup";
		mNetMan->messageServer(PROTOCOL_TCP, msgBuff, 8);
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