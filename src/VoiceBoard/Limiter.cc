/* amSynth
 * (c) 2001,2002 Nick Dowell
 */

#include "Limiter.h"

Limiter::Limiter()
{
}

Limiter::~Limiter()
{}

void
Limiter::setInput( FSource & source )
{
  this->source = &source;
}

float *
Limiter::getNFData(int nFrames)
{
  buffer = source->getFData(nFrames);

  for (int i=0; i<nFrames; i++) {	
    if (buffer[i] > 1.0) buffer[i] = 1.0;
    else if (buffer[i] < -1.0) buffer[i] = -1.0;
  }

  return buffer;
}
