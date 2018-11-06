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
		char numBuff[sizeof(int)];
		char key;
		int num;
		switch(arg.key){
		case OIS::KC_W:
			key = 'Y';
			num = 3;
			break;
		case OIS::KC_S:
			key = 'Y';
			num = -3;
			break;
		case OIS::KC_A:
			key = 'X';
			num = 3;
			break;
		case OIS::KC_D:
			key = 'X';
			num = -3;
			
			break;
		default:
			return; //return early to avoid messaging anything
			break;
		}
		//sprintf(msgBuff, "%c:%d", key, num);
		mNetMan->messageServer(PROTOCOL_TCP, msgBuff, 8);
	}
}
void NetHandler::injectUpInput(const OIS::KeyEvent& arg){
	if(!isServer){
		std::cout << "keyup" << std::endl;
		char msgBuff[] = "keyup";
		int num;
		char numBuff[sizeof(num)];
		char key;
		switch(arg.key){
		case OIS::KC_W:
			key = 'Y';
			num = -3;
			
			break;
		case OIS::KC_S:
			key = 'Y';
			num = 3;
			
			break;
		case OIS::KC_A:
			key = 'X';
			num = -3;
			
			break;
		case OIS::KC_D:
			key = 'X';
			num = 3;
			break;
		default:
			return; //return early to avoid messaging anything
			break;
		}
		memcpy(numBuff, &num, sizeof(numBuff));
		//sprintf(msgBuff, "%c:%d", key, num);
		mNetMan->messageServer(PROTOCOL_TCP, msgBuff, 6);
		//mNetMan->messageServer(PROTOCOL_TCP, numBuff, sizeof(numBuff)+1);
	}
}
void NetHandler::injectMouseDownInput(OIS::MouseButtonID id){
	if(!isServer){
		//mNetMan->messageServer(PROTOCOL_TCP, "M:1", 128);
		std::cout << "mousedown" << std::endl;
	}
}
void NetHandler::injectMouseUpInput(OIS::MouseButtonID id){
	if(!isServer){
		//mNetMan->messageServer(PROTOCOL_TCP, "M:0", 256);
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