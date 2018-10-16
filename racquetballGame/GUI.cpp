#include "GUI.h"
#include <stdio.h>
#include <stdlib.h>

GUI::GUI(){
  CEGUI::OgreRenderer* renderer = &CEGUI::OgreRenderer::bootstrapSystem();

  CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
  CEGUI::Font::setDefaultResourceGroup("Fonts");
  CEGUI::Scheme::setDefaultResourceGroup("Schemes");
  CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
  CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

  CEGUI::SchemeManager::getSingleton().createFromFile( "TaharezLook.scheme" );
  CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
  CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();

  // Uncomment these lines if you want to view the test GUI window
  /*CEGUI::Window *guiRoot = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("TextDemo.layout");
  CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(guiRoot);*/

  CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "TopGUI/Sheet");

  scoreboard = wmgr.createWindow("TaharezLook/StaticText", "TopGUI/Scoreboard");
  scoreboard->setText("Score: 0");
  scoreboard->setPosition(CEGUI::UVector2(CEGUI::UDim(0.45, 0), CEGUI::UDim(0.00, 0)));
  scoreboard->setSize(CEGUI::USize(CEGUI::UDim(0.10, 0), CEGUI::UDim(0.05, 0)));

  sheet->addChild(scoreboard);
  CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
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
  scoreboard->setText(buffer);
}
