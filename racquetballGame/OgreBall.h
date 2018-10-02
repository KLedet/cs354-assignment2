/*
-----------------------------------------------------------------------------
Filename:    OgreBall.h
-----------------------------------------------------------------------------
*/

#ifndef __OgreBall_h_
#define __OgreBall_h_

#include "BaseApplication.h"

//---------------------------------------------------------------------------

class OgreBall : public BaseApplication
{
public:
    OgreBall(void);
    virtual ~OgreBall(void);

protected:
    virtual void createScene(void);
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);

private:
  virtual bool processUnbufferedInput(const Ogre::FrameEvent& fe);
};

//---------------------------------------------------------------------------

#endif // #ifndef __OgreBall_h_

//---------------------------------------------------------------------------
