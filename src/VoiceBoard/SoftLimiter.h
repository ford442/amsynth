/* amSynth
 * (c) 2001,2002 Nick Dowell
 */

#ifndef _SOFTLIMITER_H
#define _SOFTLIMITER_H

#include "Synth--.h"

/**
 * @class SoftLimiter
 *
 * takes an input stream, which can be NF or F, and outputs an NF stream,
 * clipping the level so it stays in range. This is the only way to get
 * from an F stream back to NF..
 **/
class SoftLimiter:public NFSource, public FInput {
  public:
    SoftLimiter(float rate);
    virtual ~SoftLimiter();
    void setInput(FSource & source);
    inline float *getNFData(int nFrames);
	void isStereo(){ch=2;};

	void	Process64Samples	(float *l, float *r);
  private:
    FSource *source;
    float *buffer;
	double xpeak, attack, release, thresh;
	int ch;
};

#endif
