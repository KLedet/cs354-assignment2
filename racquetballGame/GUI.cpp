#include "GUI.h"
#include <stdio.h>
#include <stdlib.h>

void GUI::setSingleplayer(const CEGUI::EventArgs& args){
  isSingleplayer = true;
  playerModeSelected = true;
  CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChildAtIdx(4)->hide();
  CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChildAtIdx(0)->show();
}

void GUI::setMultiplayer(const CEGUI::EventArgs& args){
  isSingleplayer = false;
  playerModeSelected = true;
  CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChildAtIdx(4)->hide();
  CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChildAtIdx(3)->show();
}

void GUI::setServer(const CEGUI::EventArgs& args){
  isServer = true;
  hostStatusSelected = true;
  CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChildAtIdx(3)->hide();
  CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChildAtIdx(1)->show();
}

void GUI::setClient(const CEGUI::EventArgs& args){
  isServer = false;
  hostStatusSelected = true;
  CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChildAtIdx(3)->hide();
  CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChildAtIdx(2)->show();
}

void GUI::setHostname(const CEGUI::EventArgs& args){
  CEGUI::Window* hn = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChildAtIdx(2);
  hostname = hn->getChildAtIdx(1)->getText().c_str();
  hostnameSet = true;
  //std::cout << hostname << std::endl;
  hn->hide();
  CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChildAtIdx(1)->show();
}

GUI::GUI(){
  CEGUI::OgreRenderer* renderer = &CEGUI::OgreRenderer::bootstrapSystem();
  isSingleplayer = true;
  playerModeSelected = false;
  isServer = true;
  hostStatusSelected = false;
  hostnameSet = false;

  CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
  CEGUI::Font::setDefaultResourceGroup("Fonts");
  CEGUI::Scheme::setDefaultResourceGroup("Schemes");
  CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
  CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

  CEGUI::SchemeManager::getSingleton().createFromFile( "TaharezLook.scheme" );
  CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
  //CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();

  CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "TopGUI/Sheet");
  CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

  // SINGLE-PLAYER SCOREBOARD (IDX=0)
  CEGUI::Window* scoreboard = wmgr.createWindow("TaharezLook/StaticText", "TopGUI/Scoreboard");
  scoreboard->setText("Score: 0");
  scoreboard->setPosition(CEGUI::UVector2(CEGUI::UDim(0.45, 0), CEGUI::UDim(0.00, 0)));
  scoreboard->setSize(CEGUI::USize(CEGUI::UDim(0.10, 0), CEGUI::UDim(0.05, 0)));
  sheet->addChild(scoreboard);
  scoreboard->hide();

  // MULTIPLAYER SCOREBOARD (IDX=1)
  CEGUI::Window* doubleboard = wmgr.createWindow("DefaultWindow", "TopGUI/DoubleBoard");
  doubleboard->setPosition(CEGUI::UVector2(CEGUI::UDim(0.00, 0), CEGUI::UDim(0.00, 0)));
  doubleboard->setSize(CEGUI::USize(CEGUI::UDim(1.00, 0), CEGUI::UDim(0.05, 0)));

  CEGUI::Window* redSide = wmgr.createWindow("TaharezLook/StaticText", "TopGUI/DoubleBoard/RedSide");
  redSide->setText("Red: 0");
  redSide->setPosition(CEGUI::UVector2(CEGUI::UDim(0.00, 0), CEGUI::UDim(0.00, 0)));
  redSide->setSize(CEGUI::USize(CEGUI::UDim(0.10, 0), CEGUI::UDim(1.00, 0)));
  doubleboard->addChild(redSide);

  CEGUI::Window* blueSide = wmgr.createWindow("TaharezLook/StaticText", "TopGUI/DoubleBoard/BlueSide");
  blueSide->setText("Blue: 0");
  blueSide->setPosition(CEGUI::UVector2(CEGUI::UDim(0.90, 0), CEGUI::UDim(0.00, 0)));
  blueSide->setSize(CEGUI::USize(CEGUI::UDim(0.10, 0), CEGUI::UDim(1.00, 0)));
  doubleboard->addChild(blueSide);
  sheet->addChild(doubleboard);
  doubleboard->hide();

  // HOSTNAME ENTRY (IDX=2)
  CEGUI::Window* hostnameBox = wmgr.createWindow("DefaultWindow", "TopGUI/Hostname");
  hostnameBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.20, 0), CEGUI::UDim(0.20, 0)));
  hostnameBox->setSize(CEGUI::USize(CEGUI::UDim(0.40, 0), CEGUI::UDim(0.18, 0)));

  CEGUI::Window* hnText = wmgr.createWindow("TaharezLook/StaticText", "TopGUI/Hostname/Text");
  hnText->setPosition(CEGUI::UVector2(CEGUI::UDim(0.00, 0), CEGUI::UDim(0.00, 0)));
  hnText->setSize(CEGUI::USize(CEGUI::UDim(1.00, 0), CEGUI::UDim(0.75, 0)));
  hnText->setText("Enter hostname below:");
  hostnameBox->addChild(hnText);

  CEGUI::Window* hnEntry = wmgr.createWindow("TaharezLook/Editbox", "TopGUI/Hostname/Entry");
  hnEntry->setPosition(CEGUI::UVector2(CEGUI::UDim(0.00, 0), CEGUI::UDim(0.75, 0)));
  hnEntry->setSize(CEGUI::USize(CEGUI::UDim(1.00, 0), CEGUI::UDim(0.25, 0)));
  hnEntry->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&GUI::setHostname,this));
  hostnameBox->addChild(hnEntry);
  sheet->addChild(hostnameBox);
  hostnameBox->hide();

  // CLIENT/SERVER SELECTION (IDX=3)
  CEGUI::Window* css = wmgr.createWindow("DefaultWindow", "TopGUI/CSS");
  css->setPosition(CEGUI::UVector2(CEGUI::UDim(0.20, 0), CEGUI::UDim(0.20, 0)));
  css->setSize(CEGUI::USize(CEGUI::UDim(0.40, 0), CEGUI::UDim(0.18, 0)));

  CEGUI::Window* cssText = wmgr.createWindow("TaharezLook/StaticText", "TopGUI/CSS/Text");
  cssText->setText("Client or server?");
  cssText->setPosition(CEGUI::UVector2(CEGUI::UDim(0.00, 0), CEGUI::UDim(0.00, 0)));
  cssText->setSize(CEGUI::USize(CEGUI::UDim(1.00, 0), CEGUI::UDim(0.50, 0)));
  css->addChild(cssText);

  CEGUI::Window* cssClient = wmgr.createWindow("TaharezLook/Button", "TopGUI/CSS/ClientButton");
  cssClient->setText("Client");
  cssClient->setPosition(CEGUI::UVector2(CEGUI::UDim(0.00, 0), CEGUI::UDim(0.50, 0)));
  cssClient->setSize(CEGUI::USize(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.50, 0)));
  cssClient->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::setClient,this));
  css->addChild(cssClient);

  CEGUI::Window* cssServer = wmgr.createWindow("TaharezLook/Button", "TopGUI/CSS/ServerButton");
  cssServer->setText("Server");
  cssServer->setPosition(CEGUI::UVector2(CEGUI::UDim(0.50, 0), CEGUI::UDim(0.50, 0)));
  cssServer->setSize(CEGUI::USize(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.50, 0)));
  cssServer->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::setServer,this));
  css->addChild(cssServer);

  sheet->addChild(css);
  css->hide();

  // SINGLE/MULTIPLAYER SELECTION (IDX=4)
  CEGUI::Window* sms = wmgr.createWindow("DefaultWindow", "TopGUI/SMS");
  sms->setPosition(CEGUI::UVector2(CEGUI::UDim(0.20, 0), CEGUI::UDim(0.20, 0)));
  sms->setSize(CEGUI::USize(CEGUI::UDim(0.40, 0), CEGUI::UDim(0.18, 0)));

  CEGUI::Window* smsText = wmgr.createWindow("TaharezLook/StaticText", "TopGUI/SMS/Text");
  smsText->setText("Single player or multiplayer?");
  smsText->setPosition(CEGUI::UVector2(CEGUI::UDim(0.00, 0), CEGUI::UDim(0.00, 0)));
  smsText->setSize(CEGUI::USize(CEGUI::UDim(1.00, 0), CEGUI::UDim(0.50, 0)));
  sms->addChild(smsText);

  CEGUI::Window* smsSingle = wmgr.createWindow("TaharezLook/Button", "TopGUI/SMS/SingleButton");
  smsSingle->setText("Single player");
  smsSingle->setPosition(CEGUI::UVector2(CEGUI::UDim(0.00, 0), CEGUI::UDim(0.50, 0)));
  smsSingle->setSize(CEGUI::USize(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.50, 0)));
  smsSingle->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::setSingleplayer,this));
  sms->addChild(smsSingle);

  CEGUI::Window* smsMulti = wmgr.createWindow("TaharezLook/Button", "TopGUI/SMS/MultiButton");
  smsMulti->setText("Multiplayer");
  smsMulti->setPosition(CEGUI::UVector2(CEGUI::UDim(0.50, 0), CEGUI::UDim(0.50, 0)));
  smsMulti->setSize(CEGUI::USize(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.50, 0)));
  smsMulti->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::setMultiplayer,this));
  sms->addChild(smsMulti);

  sheet->addChild(sms);
  //sms->hide();
}

void GUI::injectTimestamps(const Ogre::FrameEvent& evt){
  CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);
}

void GUI::injectDownInput(const OIS::KeyEvent& arg){
  CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
  context.injectKeyDown((CEGUI::Key::Scan)arg.key);
  context.injectChar((CEGUI::Key::Scan)arg.text);
}

void GUI::injectUpInput(const OIS::KeyEvent& arg){
  CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)arg.key);
}

CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;

    case OIS::MB_Right:
        return CEGUI::RightButton;

    case OIS::MB_Middle:
        return CEGUI::MiddleButton;

    default:
        return CEGUI::LeftButton;
    }
}

void GUI::injectMouseDownInput(OIS::MouseButtonID id){
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id));
}

void GUI::injectMouseUpInput(OIS::MouseButtonID id){
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertButton(id));
}

void GUI::injectMouseMovement(const OIS::MouseEvent &arg){
  CEGUI::GUIContext &sys = CEGUI::System::getSingleton().getDefaultGUIContext();
  sys.injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
  // Scroll wheel.
  if (arg.state.Z.rel)
    sys.injectMouseWheelChange(arg.state.Z.rel / 120.0f);
}

void GUI::updateScore(int redCount, int blueCount){
  if(isSingleplayer){
    char buffer[20];
    sprintf(buffer, "Score: %d", redCount);
    CEGUI::Window* sb = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChildAtIdx(0);
    sb->setText(buffer);
  } else {
    CEGUI::Window* sb = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChildAtIdx(1);
    char redBuffer[20];
    sprintf(redBuffer, "Red: %d", redCount);
    sb->getChildAtIdx(0)->setText(redBuffer);
    char blueBuffer[20];
    sprintf(blueBuffer, "Blue: %d", blueCount);
    sb->getChildAtIdx(1)->setText(blueBuffer);
  }

}
