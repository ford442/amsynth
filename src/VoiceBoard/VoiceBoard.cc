/* amSynth
 * (c) 2001-2004 Nick Dowell
 */

#include "VoiceBoard.h"

#include <iostream>

VoiceBoard::VoiceBoard(int rate, VoiceBoardProcessMemory *mem):
	// call object constructors with parameters
	
	lfo1			(rate, mem->lfo_osc_1),
	osc1			(rate, mem->osc_1),
	osc2			(rate, mem->osc_2),
	filter			(rate), 
	filter_env		(rate,mem->filter_env), 
	amp_env			(rate,mem->amp_env)
{
	this->rate = rate;
	this->mem = mem;
}

VoiceBoard::~VoiceBoard()
{
}

Parameter &
VoiceBoard::parameter( string name )
{
	return _preset->getParameter( name );
}

void
VoiceBoard::init()
{
	/*
	 * LFO
	 */
	lfo1.setWaveform( parameter("lfo_waveform") );
	parameter("lfo_freq").addUpdateListener (*this);
	
	parameter("freq_mod_amount").addUpdateListener (*this);

	/* 
	 * oscillator section
	 */
	osc1.setWaveform( parameter("osc1_waveform") );
	osc1.setSync( parameter("osc2_sync"), osc2 );
	osc2.setWaveform( parameter("osc2_waveform") );
	parameter("osc1_pulsewidth").addUpdateListener (*this);
	parameter("osc2_detune").addUpdateListener (*this);
	parameter("osc2_range").addUpdateListener (*this);
	parameter("osc2_pulsewidth").addUpdateListener (*this);
	
	/*
	 * osc mix section
	 */
	parameter("osc_mix").addUpdateListener (*this);
	parameter("osc_mix_mode").addUpdateListener (*this);
	
	/*
	 * filter section
	 */
	filter_env.setAttack( parameter("filter_attack") );
	filter_env.setDecay( parameter("filter_decay") );
	filter_env.setSustain( parameter("filter_sustain") );
	filter_env.setRelease( parameter("filter_release") );
	
	parameter("filter_mod_amount").addUpdateListener (*this);
	parameter("filter_env_amount").addUpdateListener (*this);
	parameter("filter_resonance").addUpdateListener (*this);
	parameter("filter_cutoff").addUpdateListener (*this);
	
	/* 
	 * amp section
	 */
	amp_env.setAttack( parameter("amp_attack") );
	amp_env.setDecay( parameter("amp_decay") );
	amp_env.setSustain( parameter("amp_sustain") );
	amp_env.setRelease( parameter("amp_release") );
	
	parameter("amp_mod_amount").addUpdateListener (*this);
}

void
VoiceBoard::UpdateParameter	(Param param, float value)
{
	switch (param)
	{
	case kAmpModAmount:	mAmpModAmount = (value+1.0)/2.0;break;
	case kLFOFreq:		mLFO1Freq = value;		break;
	case kFreqModAmount:	mFreqModAmount=(value/2.0)+0.5;	break;
	case kOsc1Pulsewidth:	mOsc1PulseWidth = value;	break;
	case kOsc2Pulsewidth:	mOsc2PulseWidth = value;	break;
	case kOsc2Octave:	mOsc2Octave = value;		break;
	case kOsc2Detune:	mOsc2Detune = value;		break;
	case kFilterModAmount:	mFilterModAmt = (value+1.0)/2.0;break;
	case kFilterEnvAmount:	mFilterEnvAmt = value;		break;
	case kFilterCutoff:	mFilterCutoff = value;		break;
	case kFilterResonance:	mFilterRes = value;		break;
	case kOscMixRingMod:
	case kOscMix:
		mRingModAmt = parameter("osc_mix_mode").getControlValue ();
		if (mRingModAmt == 0)
		{
			float mix = parameter("osc_mix").getControlValue ();
			mOsc1Vol = (1-mix)/2.0;
			mOsc2Vol = (mix+1)/2.0;
		}
		else mOsc1Vol = mOsc2Vol = 0.0;
		break;
	default: break;
	}
}

void
VoiceBoard::Process64SamplesMix	(float *buffer, float vol)
{
	mPitchBend = pitch_bend->getFData(1)[0];
	
	float *lfo1buf = mem->lfo_osc_1;
	lfo1.Process64Samples (lfo1buf, mLFO1Freq, 0);

	float osc1freq = mPitchBend*mKeyPitch * ( mFreqModAmount*(lfo1buf[0]+1.0) + 1.0 - mFreqModAmount );
	float osc1pw = mOsc1PulseWidth;

	float osc2freq = osc1freq * mOsc2Detune * mOsc2Octave;
	float osc2pw = mOsc2PulseWidth;

	float env_f = *filter_env.getNFData (64);
        float cutoff = mKeyPitch * env_f * mFilterEnvAmt + ( mKeyPitch * mKeyVelocity * mFilterCutoff ) * ( (lfo1buf[0]*0.5 + 0.5) * mFilterModAmt + 1-mFilterModAmt );

	float *osc1buf = mem->osc_1;
	float *osc2buf = mem->osc_2;
	osc1.Process64Samples (osc1buf, osc1freq, osc1pw);
	osc2.Process64Samples (osc2buf, osc2freq, osc2pw);

	for (int i=0; i<64; i++)
		osc1buf[i] =
			mOsc1Vol * osc1buf[i] +
			mOsc2Vol * osc2buf[i] +
			mRingModAmt * osc1buf[i]*osc2buf[i];

	filter.Process64Samples (osc1buf, cutoff, mFilterRes);
	
	float *ampenvbuf = amp_env.getNFData (64);
	for (int i=0; i<64; i++) 
		osc1buf[i] = osc1buf[i]*ampenvbuf[i]*mKeyVelocity *
			( ((lfo1buf[i]*0.5)+0.5)*mAmpModAmount + 1-mAmpModAmount);

	for (int i=0; i<64; i++) buffer[i] += (osc1buf[i] * vol);
}

void VoiceBoard::setPitchWheel(FSource & source)
{
	pitch_bend = &source;
}

int 
VoiceBoard::getState()
{
	return amp_env.getState();
}

void 
VoiceBoard::triggerOn()
{
	amp_env.triggerOn();
	filter_env.triggerOn();
}

void 
VoiceBoard::triggerOff()
{
	amp_env.triggerOff();
	filter_env.triggerOff();
}

void
VoiceBoard::reset()
{
	amp_env.reset();
	filter_env.reset();
	osc1.reset();
	osc2.reset();
	filter.reset();
	lfo1.reset();
}
void 
VoiceBoard::setFrequency(float frequency)
{
	mKeyPitch = frequency;
}

void 
VoiceBoard::setVelocity(float velocity)
{
	mKeyVelocity = velocity;
}
