/* amSynth
 * (c)2001,2002 Nick Dowell
 */

#ifndef _PROCESSANDHOLD_H
#define _PROCSESANDHOLD_H

#include "Synth--.h"

class ProcessAndHold : public FSource, public FInput
{
public:
	float * getFData(int nFrames);
	void setInput( FSource & source );
	void process(int nFrames);
private:
	FSource * input;
	float * buffer;
};

#endif
