/* amSynth
 * (c) 2001,2002 Nick Dowell
 */

#include "Reverb.h"

Reverb::Reverb()
{
//	model.mute();
}

void
Reverb::update()
{
	model.setmode( modeParam->getControlValue() );
	model.setroomsize( roomSizeParam->getControlValue() );
	model.setdamp( dampParam->getControlValue() );
	model.setwet( wetParam->getControlValue() );
	model.setdry( 1-wetParam->getControlValue() );
//	model.setdry( dryParam->getControlValue() );
	model.setwidth( widthParam->getControlValue() );
	//model.update();
}

void
Reverb::Alloc	(int nFrames)
{
	outbufferL = new float[nFrames];
	outbufferR = new float[nFrames];
	outBuffer  = new float[nFrames*2];
}

void
Reverb::Process64Samples	(float *in, float *l, float *r)
{
	model.processreplace (in, in, l, r, 64, 1);
}
