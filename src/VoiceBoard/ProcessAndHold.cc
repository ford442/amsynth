/* amSynth
 * (c)2001,2002 Nick Dowell
 */

#include "ProcessAndHold.h"

void
ProcessAndHold::setInput( FSource & source )
{
	input = &source;
}

void
ProcessAndHold::process(int nFrames)
{
	buffer = input->getFData(nFrames);}

float *
ProcessAndHold::getFData(int)
{
	return buffer;
}
