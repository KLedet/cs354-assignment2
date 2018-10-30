#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
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
  void updateScore(int rallyCount);
  void closeMenu();
};
