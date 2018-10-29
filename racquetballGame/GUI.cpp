#include "GUI.h"
#include <stdio.h>
#include <stdlib.h>

void GUI::closeMenu(){
  CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChildAtIdx(1)->hide();
  CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
}

GUI::GUI(){
  CEGUI::OgreRenderer* renderer = &CEGUI::OgreRenderer::bootstrapSystem();

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

  // DEFAULT SCOREBOARD (IDX=0)
  CEGUI::Window* scoreboard = wmgr.createWindow("TaharezLook/StaticText", "TopGUI/Scoreboard");
  scoreboard->setText("Score: 0");
  scoreboard->setPosition(CEGUI::UVector2(CEGUI::UDim(0.45, 0), CEGUI::UDim(0.00, 0)));
  scoreboard->setSize(CEGUI::USize(CEGUI::UDim(0.10, 0), CEGUI::UDim(0.05, 0)));
  sheet->addChild(scoreboard);
  scoreboard->hide();

  // CLIENT/SERVER SELECTION (IDX=1)
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
  cssClient->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::closeMenu,this));
  css->addChild(cssClient);

  CEGUI::Window* cssServer = wmgr.createWindow("TaharezLook/Button", "TopGUI/CSS/ServerButton");
  cssServer->setText("Server");
  cssServer->setPosition(CEGUI::UVector2(CEGUI::UDim(0.50, 0), CEGUI::UDim(0.50, 0)));
  cssServer->setSize(CEGUI::USize(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.50, 0)));
  cssServer->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::closeMenu,this));
  css->addChild(cssServer);

  sheet->addChild(css);
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

void GUI::updateScore(int rallyCount){
  char buffer[20];
  sprintf(buffer, "Score: %d", rallyCount);
  CEGUI::Window* sb = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChildAtIdx(0);
  sb->setText(buffer);

}
