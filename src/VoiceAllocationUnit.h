/* amSynth
 * (c) 2001,2002 Nick Dowell
 */

#ifndef _VOICEALLOCATIONUNIT_H
#define _VOICEALLOCATIONUNIT_H

#include "PresetController.h"
#include "VoiceBoard/VoiceBoard.h"
#include "VoiceBoard/Adder.h"
#include "VoiceBoard/SoftLimiter.h"
#include "VoiceBoard/FValue.h"
#include "VoiceBoard/NFValue.h"
#include "VoiceBoard/Reverb.h"
#include "VoiceBoard/Distortion.h"
#include "AudioOutput.h"
#include "VoiceAllocationUnit.h"
#include "UpdateListener.h"
#include "Config.h"

#include <pthread.h>

class VoiceAllocationUnit
{
public:
  VoiceAllocationUnit( Config & config );
  virtual ~VoiceAllocationUnit(){};
  
  void	AllocateMemory	(int nFrames);
  
  void noteOn(int note, float velocity);
  void noteOff(int note);
  void pwChange( float value );
  void setPresetController(PresetController & p_c)
  {_presetController = &p_c; };
  void setPreset(Preset & preset);
  void sustainOn()
  { sustain = 1; };
  void sustainOff();
  void killAllVoices();
	void		set_max_voices	( int voices );

	void	Process64Samples	(float *l, float *r);

private:
  int max_voices;
  void purgeVoices();
  SoftLimiter limiter;
  FValue pw_val;
  float _pitch[128];
  float outBuffer[BUF_SIZE*2];
  char keyPressed[128], sustain;
  bool	active[128];
  VoiceBoard *_voices[128];
  Preset *_preset;
  PresetController *_presetController;
  Reverb reverb;
  Distortion distortion;
  Config *config;
};

#endif
