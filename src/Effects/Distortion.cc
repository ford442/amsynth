/* amSynth
 * (c) 2001,2002 Nick Dowell
 */

#include "Distortion.h"

Distortion::Distortion()
{
	driveParam = 0;
	crunchParam = 0;
    drive = 1;
    crunch = 1 / 4;
	done = 0;
}

void 
Distortion::setDrive(Parameter & param)
{
    driveParam = &param;
    driveParam->addUpdateListener(*this);
    update();
}

void 
Distortion::setCrunch(Parameter & param)
{
    crunchParam = &param;
    crunchParam->addUpdateListener(*this);
    update();
}

void 
Distortion::update()
{
    if(driveParam){
		drive = driveParam->getControlValue();
//		cout << "drive: " << drive << endl;
	}
    // crunch values of 16 = nice and clean, 2 = diiirty!
    if(crunchParam)
	{
//		crunch = 1/(2*(crunchParam->getControlValue()+1));
//		crunch*=crunch;
//		crunch*=crunch;
//		crunch*=8;
		/* LIMITDISTORT
		crunch=crunchParam->getControlValue();
		*/
/*		// LOGDISTORT
		crunch=crunchParam->getControlValue()*2;
		crunch*=crunch;crunch*=crunch;*/
		// EXPDISTORT
		crunch=1-crunchParam->getControlValue();
	}
}

void
Distortion::Process64Samples	(float *buffer)
{
	register float x, s;
	if (crunch == 0) crunch = 0.01;
	
	for (int i=0; i<64; i++)
	{
		x = buffer[i]*drive;
		if(x<0) s=-1; else s=1;
		x*=s;
		x = pow (x, crunch);
		buffer[i] = x*s;
	}
}
