#include "NetHandler.h"

NetHandler::NetHandler(NetManager* netManager){
	mNetMan = netManager;
	isServer = false;
	connected = false;
}

NetHandler::~NetHandler(){
	delete mNetMan;
}

void NetHandler::addObject(char key, GameObject* obj){
	objList[key] = obj;
}
void NetHandler::injectDownInput(const OIS::KeyEvent& arg){
	if(!isServer){
		//std::cout<< "Keydown" << std::endl;
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
		//std::cout << "keyup" << std::endl;
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
		//std::cout << "mousedown" << std::endl;
	}
}
void NetHandler::injectMouseUpInput(OIS::MouseButtonID id){
	if(!isServer){
		char msgBuff[] = "mouseup";
		mNetMan->messageServer(PROTOCOL_TCP, msgBuff, 8);
		//std::cout << "mouseup" << std::endl;
	}
}

void NetHandler::sendTransform(btTransform tr, char objID){
	char cmdBuff[] = "transform";
	char idBuff[2];
	btTransformData data;
	tr.serializeFloat(data);
	/*
	Ogre::Vector3 pos = node->getPosition();
	std::cout << sizeof(pos) << std::endl;
	Ogre::Quaternion rot = node->getOrientation();
	std::cout << sizeof(rot) << std::endl;*/
	int message_length = sizeof(data);
	/*for (int i = 0; i < 4; i++){
		std::cout << data.m_origin.m_floats[i] << " " ;
	}

	std::cout << std::endl;*/
	char msgBuff[message_length];
	idBuff[0] = objID;
	idBuff[1] = 0;
	/*
    memcpy(msgBuff, &pos, sizeof(pos));
    memcpy(msgBuff + sizeof(pos), &rot, sizeof(rot));
    */
    memcpy(msgBuff, &data, sizeof(data));
    mNetMan->messageClient(PROTOCOL_TCP, 0, cmdBuff, 10);
    mNetMan->messageClient(PROTOCOL_TCP, 0, idBuff, 2);
	mNetMan->messageClient(PROTOCOL_TCP, 0, msgBuff, message_length);
}

void NetHandler::sendScore(int score1, int score2){
	char cmdBuff[] = "score";
	/*
	Ogre::Vector3 pos = node->getPosition();
	std::cout << sizeof(pos) << std::endl;
	Ogre::Quaternion rot = node->getOrientation();
	std::cout << sizeof(rot) << std::endl;*/
	/*
    memcpy(msgBuff, &pos, sizeof(pos));
    memcpy(msgBuff + sizeof(pos), &rot, sizeof(rot));
    */
    int message_length = sizeof(score1) + sizeof(score2);
	char msgBuff[message_length];
    memcpy(msgBuff, &score1, sizeof(score1));
    memcpy(msgBuff + sizeof(score1), &score2, sizeof(score2));
    mNetMan->messageClient(PROTOCOL_TCP, 0, cmdBuff, 6);
	mNetMan->messageClient(PROTOCOL_TCP, 0, msgBuff, message_length);
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