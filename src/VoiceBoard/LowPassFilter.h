/* amSynth
 * (c) 2001,2002 Nick Dowell
 */

#ifndef _LOWPASSFILTER_H
#define _LOWPASSFILTER_H

#include "Synth--.h"
#include "../UpdateListener.h"
#include "../Parameter.h"

/**
 * A 24 dB/ocatave resonant low-pass filter.
 **/
class LowPassFilter : public UpdateListener
{
public:
	LowPassFilter(int rate);
	virtual ~LowPassFilter(){};
	
	void setCutoff( Parameter & param );
	/**
	 * @param param The Parameter which controls the resonance of the filter.
	 * i.e. 'Q'
	 */
	void setResonance( Parameter & param );
	/**
	 * Reset the filter - clear anything in the delay units of the filter.
	 */
	void reset();
	void update();

	void	Process64Samples	(float*, float cutoff, float res);
private:
	Parameter * cutoff_param;
	Parameter * res_param;
	int rate;
	float nyquist;
	double a0, a1, a2, b1, b2, res;
	float f, k, p, r, max;
	double d1, d2, d3, d4, x, y;
};

#endif
