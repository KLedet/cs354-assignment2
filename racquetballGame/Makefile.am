noinst_HEADERS = BaseApplication.h OgreBall.h Ball.h Room.h Simulator.h GameObject.h GUI.h Player.h 

## May want these at a later time: ControlListener.h Overlay.h Player.h PlayerCamera.h PlayingField.h  Wall.h OgreMotionState.h GameObject.h

bin_PROGRAMS = OgreBall
OgreBall_CPPFLAGS = -I$(top_srcdir)
OgreBall_SOURCES = BaseApplication.cpp OgreBall.cpp Simulator.cpp GUI.cpp Ball.cpp GameObject.cpp Player.cpp 

## May want these at a later time: Ball.cpp ControlListener.cpp  Overlay.cpp Player.cpp PlayerCamera.cpp PlayingField.cpp  Wall.cpp GameObject.cpp

OgreBall_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) -I/lusr/opt/cegui-0.8.7/include/cegui-0
OgreBall_LDADD = $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS) -L/lusr/opt/cegui-0.8.7/lib -lCEGUIBase-0 -lCEGUIOgreRenderer-0
OgreBall_LDFLAGS = -lOgreOverlay -lboost_system -R/lusr/lib/cegui-0.8

EXTRA_DIST = buildit makeit
AUTOMAKE_OPTIONS = foreign
