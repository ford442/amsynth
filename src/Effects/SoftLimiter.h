/* amSynth
 * (c) 2001-2004 Nick Dowell
 */

#ifndef _SOFTLIMITER_H
#define _SOFTLIMITER_H

class SoftLimiter
{
  public:
    SoftLimiter(float rate);
    virtual ~SoftLimiter();
	void isStereo(){ch=2;};

	void	Process64Samples	(float *l, float *r);
  private:
    float *buffer;
	double xpeak, attack, release, thresh;
	int ch;
};

#endif
