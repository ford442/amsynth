/* amSynth
 * (c) 2001-2004 Nick Dowell
 */

#ifndef _VOICEBOARD_H
#define _VOICEBOARD_H

#include "../PresetController.h"
#include "../Preset.h"
#include "Synth--.h"
#include "ADSR.h"
#include "Oscillator.h"
#include "FValue.h"
#include "NFValue.h"
#include "LowPassFilter.h"
#include "ProcessAndHold.h"
#include "FreqControlSignal.h"
#include "AmpSection.h"
#include "../Preset.h"
#include "../Parameter.h"

class VoiceBoardProcessMemory
{
public:
	VoiceBoardProcessMemory	(int bufsize)
	{
		osc_1 = new float[bufsize];
		osc_2 = new float[bufsize];
		lfo_osc_1 = new float[bufsize];
		key_pitch = new float[bufsize];
		lfo_freq = new float[bufsize];
		filter_env = new float[bufsize];
		amp_env = new float[bufsize];
	}
	VoiceBoardProcessMemory	()
	{
		delete[] osc_1;
		delete[] osc_2;
		delete[] lfo_osc_1;
		delete[] key_pitch;
		delete[] lfo_freq;
		delete[] filter_env;
		delete[] amp_env;
	}
		
	float*	osc_1;
	float*	osc_2;
	float*	lfo_osc_1;
	float*	key_pitch;
	float*	lfo_freq;
	float*	filter_env;
	float*	amp_env;
};
	

/**
 * the VoiceBoard is what makes the nice noises... ;-)
 *
 * one VoiceBoard is a 'voice' in its own right, and play only one note at a 
 * time. the VoiceAllocationUnit decides which voices do what etc...
 **/

class VoiceBoard : public UpdateListener
{
public:
	VoiceBoard(int rate, VoiceBoardProcessMemory *mem);
	virtual ~VoiceBoard();
	void init();
	void	Process64SamplesMix	(float *buffer, float vol);
	int getState();
	void triggerOn();
	void triggerOff();
	void setVelocity(float velocity);
	void setFrequency(float frequency);
	void setPreset(Preset & preset) { _preset = &preset; };
	void setPitchWheelParam(Parameter & param);
	void setPitchWheel(FSource & source);
	void reset();

	void	UpdateParameter	(Param, float);
private:

	VoiceBoardProcessMemory	*mem;
	
	Parameter &	parameter( string name );
	int rate;
	float *buffer;
	Preset *_preset;

	float			mKeyVelocity;

	// pitch control section
	FSource*		pitch_bend;
	FValue 			key_pitch;
	FreqControlSignal 	master_freq;

	// modulation section
	Oscillator 		mod_lfo_real;
	FValue 			lfo_freq;
	ProcessAndHold 		mod_lfo;
	
	// oscillator section
	Oscillator 		osc1, osc2;
	float			mOsc1PulseWidth;
	float			mOsc2PulseWidth;
	float			mOsc1Vol;
	float			mOsc2Vol;
	float			mRingModAmt;
	float			mOsc2Octave;
	float			mOsc2Detune;
	
	// filter section
	float			mFilterEnvAmt,
				mFilterModAmt,
				mFilterCutoff,
				mFilterRes;
	LowPassFilter 		filter;
	ADSR 			filter_env;
	
	// amp section
	ADSR 			amp_env;
	AmpSection 		amp;
};

#endif
