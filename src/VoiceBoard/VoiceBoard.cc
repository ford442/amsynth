
/* amSynth
 * (c) 2001-2004 Nick Dowell
 */

#include "VoiceBoard.h"

#include <iostream>

VoiceBoard::VoiceBoard(int rate, VoiceBoardProcessMemory *mem):
	// call object constructors with parameters
	
	key_pitch		(mem->key_pitch),
	freq			(mem->freq),
	freq_mod_mult		(mem->freq_mod_mult),
	freq_mod_mix_amount	(mem->freq_mod_mix_amount),
	freq_mod_mix		(mem->freq_mod_mix),
	mod_lfo_real		(rate, mem->lfo_osc_1),	
	lfo_freq		(mem->lfo_freq),	
	mod_mult		(mem->mod_mult),
	mod_add			(mem->mod_add),
	osc1			(rate, mem->osc_1),
	osc2			(rate, mem->osc_2),
	osc2_freq		(mem->osc2_freq),
	osc1_pw			(mem->osc1_pw),
	osc2_detune		(mem->osc_2_detune),
	osc2_range		(mem->osc_2_range),
	osc1_pulsewidth_control	(mem->osc_1_pulsewidth),
	osc2_pulsewidth_control	(mem->osc_2_pulsewidth),
	osc1_pwm_amt		(mem->osc_1_pwm_amount),
	osc1_pw_mixer		(mem->osc1_pw_mixer),
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
	lfo_freq.setParameter( parameter("lfo_freq") );
	mod_lfo_real.setInput( lfo_freq );
	mod_lfo_real.setWaveform( parameter("lfo_waveform") );
	
	mod_lfo.setInput( mod_lfo_real );
	
	/*
	 * pitch control section
	 */
	master_freq.setLFO( mod_lfo );
	master_freq.setPitchBend( *pitch_bend );
	master_freq.setKeyPitch( key_pitch );
	master_freq.setModAmount( parameter("freq_mod_amount") );

	/* 
	 * oscillator section
	 */
	osc1_pulsewidth_control.setParameter( parameter("osc1_pulsewidth") );
	
	osc1.setPulseWidth( osc1_pulsewidth_control );
	osc1.setWaveform( parameter("osc1_waveform") );
//	osc1.setInputSig( &master_freq );
	osc1.setInput( master_freq );
	
	osc1.setSync( parameter("osc2_sync"), osc2 );

	osc2_freq.addInput( master_freq );
	osc2_detune.setParameter( parameter("osc2_detune") );
	osc2_freq.addInput( osc2_detune );
	osc2_range.setParameter( parameter("osc2_range") );
	osc2_freq.addInput( osc2_range );

	osc2.setWaveform( parameter("osc2_waveform") );
	osc2_pulsewidth_control.setParameter( parameter("osc2_pulsewidth") );
	osc2.setPulseWidth( osc2_pulsewidth_control );
	osc2.setInput( osc2_freq );

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
	
	//amp.setInput( filter );
	amp.setLFO( mod_lfo );
	amp.setEnvelope( amp_env );
	amp.setModAmount( parameter("amp_mod_amount") );
}

void
VoiceBoard::update	()
{
	mRingModAmt = parameter("osc_mix_mode").getControlValue ();
	if (mRingModAmt == 0)
	{
		float mix = parameter("osc_mix").getControlValue ();
		mOsc1Vol = (1-mix)/2.0;
		mOsc2Vol = (mix+1)/2.0;
	}
	else 
		mOsc1Vol = mOsc2Vol = 0.0;

	mFilterModAmt = (parameter("filter_mod_amount").getControlValue ()+1.0)/2.0;
	mFilterEnvAmt = parameter("filter_env_amount").getControlValue ();
	mFilterCutoff = parameter("filter_cutoff").getControlValue ();
	mFilterRes = parameter("filter_resonance").getControlValue ();
}

void
VoiceBoard::Process64SamplesMix	(float *buffer, float vol)
{
	mod_lfo.process (64);
	master_freq.process (64);

	float env_f = *filter_env.getNFData (64);
        float cutoff = mem->key_pitch[0] * env_f * mFilterEnvAmt + ( mem->key_pitch[0] * mKeyVelocity * mFilterCutoff ) * ( (mem->lfo_osc_1[0]*0.5 + 0.5) * mFilterModAmt + 1-mFilterModAmt );

	float *osc1buf = osc1.getNFData (64);
	float *osc2buf = osc2.getNFData (64);

	for (int i=0; i<64; i++)
		osc1buf[i] =
			mOsc1Vol * osc1buf[i] +
			mOsc2Vol * osc2buf[i] +
			mRingModAmt * osc1buf[i]*osc2buf[i];

	filter.Process64Samples (osc1buf, cutoff, mFilterRes);
	amp.Process64Samples (osc1buf);
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
	mod_lfo_real.reset();
}
void 
VoiceBoard::setFrequency(float frequency)
{
	key_pitch.setValue( frequency );
}

void 
VoiceBoard::setVelocity(float velocity)
{
	mKeyVelocity = velocity;
	amp.setVelocity( velocity );
}
