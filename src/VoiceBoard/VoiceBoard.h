/* amSynth
 * (c) 2001,2002 Nick Dowell
 */

#ifndef _VOICEBOARD_H
#define _VOICEBOARD_H

#include "../PresetController.h"
#include "../Preset.h"
#include "Synth--.h"
#include "ADSR.h"
#include "Amplifier.h"
#include "Oscillator.h"
#include "FValue.h"
#include "NFValue.h"
#include "Adder.h"
#include "Multiplier.h"
#include "LowPassFilter.h"
#include "Mixer.h"
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
		lfo_osc_1_acc = new float[bufsize];
		lfo_osc_1 = new float[bufsize];
		key_pitch = new float[bufsize];
		freq_mod_mix_amount = new float[bufsize];
		lfo_freq = new float[bufsize];
		osc_2_detune = new float[bufsize];
		osc_2_range = new float[bufsize];
		osc1_pulsewidth = new float[bufsize];
		osc_1_pulsewidth = new float[bufsize];
		osc2_pulsewidth = new float[bufsize];
		osc_2_pulsewidth = new float[bufsize];
		osc_1_pwm_amount = new float[bufsize];
		mod_add = new float[bufsize];
		filter_env = new float[bufsize];
		amp_env = new float[bufsize];
		freq = new float[bufsize];
		freq_mod_mult = new float[bufsize];
		mod_mult = new float[bufsize];
		osc2_freq = new float[bufsize];
		osc1_pw = new float[bufsize];
		freq_mod_mix = new float[bufsize];
		osc1_pw_mixer = new float[bufsize];
	}
	VoiceBoardProcessMemory	()
	{
		delete[] osc_1;
		delete[] osc_2;
		delete[] lfo_osc_1_acc;
		delete[] lfo_osc_1;
		delete[] key_pitch;
		delete[] freq_mod_mix_amount;
		delete[] lfo_freq;
		delete[] osc_2_detune;
		delete[] osc_2_range;
		delete[] osc1_pulsewidth;
		delete[] osc_1_pulsewidth;
		delete[] osc2_pulsewidth;
		delete[] osc_2_pulsewidth;
		delete[] osc_1_pwm_amount;
		delete[] mod_add;
		delete[] filter_env;
		delete[] amp_env;
		delete[] freq;
		delete[] freq_mod_mult;
		delete[] mod_mult;
		delete[] osc2_freq;
		delete[] osc1_pw;
		delete[] freq_mod_mix;
		delete[] osc1_pw_mixer;
	}
		
	float*	osc_1;
	float*	osc_2;
	float*	lfo_osc_1_acc;
	float*	lfo_osc_1;
	float*	key_pitch;
	float*	freq_mod_mix_amount;
	float*	lfo_freq;
	float*	osc_2_detune;
	float*	osc_2_range;
	float*	osc1_pulsewidth;
	float*	osc_1_pulsewidth;
	float*	osc2_pulsewidth;
	float*	osc_2_pulsewidth;
	float*	osc_1_pwm_amount;
	float*	mod_add;
	float*	filter_env;
	float*	amp_env;
	float*	freq;
	float*	freq_mod_mult;
	float*	mod_mult;
	float*	osc2_freq;
	float*	osc1_pw;
	float*	freq_mod_mix;
	float*	osc1_pw_mixer;
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

	void	update	();
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
	Multiplier 		freq, freq_mod_mult;
	NFValue 		freq_mod_mix_amount;
	Mixer 			freq_mod_mix;
	FreqControlSignal 	master_freq;

	// modulation section
	Oscillator 		mod_lfo_real;
	FValue 			lfo_freq;
	Multiplier 		mod_mult;
	Adder 			mod_add;
	ProcessAndHold 		mod_lfo;
	
	// oscillator section
	Oscillator 		osc1, osc2;
	float			mOsc1Vol;
	float			mOsc2Vol;
	float			mRingModAmt;
	Multiplier 		osc2_freq, osc1_pw;
	FValue			osc2_detune, osc2_range;
	NFValue 		osc1_pulsewidth_control, osc2_pulsewidth_control, osc1_pwm_amt;
	Mixer			osc1_pw_mixer;
	
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
