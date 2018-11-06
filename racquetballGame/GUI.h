#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <string>
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

class GUI {

public:
  GUI();
  void injectTimestamps(const Ogre::FrameEvent& evt);
  void injectDownInput(const OIS::KeyEvent& arg);
  void injectUpInput(const OIS::KeyEvent& arg);
  void injectMouseDownInput(OIS::MouseButtonID id);
  void injectMouseUpInput(OIS::MouseButtonID id);
  void injectMouseMovement(const OIS::MouseEvent &arg);
  void updateScore(int redCount, int blueCount=0);

  bool isSingleplayer;
  bool playerModeSelected;
  bool isServer;
  bool hostStatusSelected;
  bool hostnameSet;
  const char* hostname;

  void setSingleplayer(const CEGUI::EventArgs& args);
  void setMultiplayer(const CEGUI::EventArgs& args);
  void setServer(const CEGUI::EventArgs& args);
  void setClient(const CEGUI::EventArgs& args);
  void setHostname(const CEGUI::EventArgs& args);

};
