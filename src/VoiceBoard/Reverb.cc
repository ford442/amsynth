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

float *
Reverb::getNFData(int nFrames)
{
	inbuffer = input->getFData(nFrames);
	model.processreplace( inbuffer, inbuffer, outbufferL, outbufferR, nFrames, 1 );
	// combine the channel buffers into a stereo buffer
	register int ch = 0;
	register int idx = 0;
	for(int i=0; i<nFrames*2; i++)
		outBuffer[i] = ( (ch=1-ch) ? outbufferL[idx] : outbufferR[idx++] );
	return outBuffer;
}
