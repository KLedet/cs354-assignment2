noinst_HEADERS = BaseApplication.h OgreBall.h Ball.h Room.h

## May want these at a later time: ControlListener.h GameObject.h OgreMotionState.h Overlay.h Player.h PlayerCamera.h PlayingField.h Simulator.h Wall.h

bin_PROGRAMS = OgreBall
OgreBall_CPPFLAGS = -I$(top_srcdir)
OgreBall_SOURCES = BaseApplication.cpp OgreBall.cpp

## May want these at a later time: Ball.cpp ControlListener.cpp GameObject.cpp Overlay.cpp Player.cpp PlayerCamera.cpp PlayingField.cpp Simulator.cpp Wall.cpp

OgreBall_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS)
OgreBall_LDADD = $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS)
OgreBall_LDFLAGS = -lOgreOverlay -lboost_system

EXTRA_DIST = buildit makeit
AUTOMAKE_OPTIONS = foreign
