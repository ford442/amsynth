/* amSynth
 * (c) 2001,2002 Nick Dowell
 */

#ifndef _REVERB_H
#define _REVERB_H

#include "revmodel.hpp"
#include "../UpdateListener.h"
#include "../Parameter.h"

class Reverb : public UpdateListener
{
  public:
    Reverb();

	void	Alloc	(int nFrames);
    
	void update();
	void setRoomSize( Parameter & param )
	{ roomSizeParam = &param; param.addUpdateListener(*this); };
	void setDamp( Parameter & param )
	{ dampParam = &param; param.addUpdateListener(*this); };
	void setWet( Parameter & param )
	{ wetParam = &param; param.addUpdateListener(*this); };
	void setDry( Parameter & param )
	{ dryParam = &param; param.addUpdateListener(*this); };
	void setWidth( Parameter & param )
	{ widthParam = &param; param.addUpdateListener(*this); };
	void setMode( Parameter & param )
	{ modeParam = &param; param.addUpdateListener(*this); };
	void mute()
	{ model.mute(); };

	void	Process64Samples	(float *in, float *l, float *r);

  private:
	revmodel model;
	float *inbuffer, *outbufferL, *outbufferR, *outBuffer;
	Parameter *roomSizeParam, *dampParam, *wetParam, *dryParam, *widthParam, *modeParam;
};

#endif
