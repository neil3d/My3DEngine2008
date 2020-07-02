#pragma once

#include "..\..\External\fft\FFT.h"

#define NX 64
#define NY 64
#define NXTIMESNY NX*NY

#define INV_SQRT_TWO (1.0f)/sqrt(2.0f)

#define GRAV_CONSTANT	9.81f //gravitational constant metric

#define PI              3.141592653589793238462643

typedef struct {
	float real,imag;
} COMPLEX;


class FFTWave
{
public:
	FFTWave(float cycle_length);
	~FFTWave(void);

	void	calculate_ho();
	void	pre_choppy();
	void	prep_loop();
	void    idle(float deltaTime);

	float hold_horizontal[NX][NY][4];//store k[0],k[1],klen,klen*klen
	float sea[NX][NY][4];
	COMPLEX c[NX][NY];
	COMPLEX mH0[NX][NY];
	COMPLEX mDeltaX[NX][NY];
	COMPLEX mDeltaY[NX][NY];
	float displayXY[NX][NY][2]; // holds the actual x and y for display
	float WorldSizeX;
	float WorldSizeY;

	fftwf_plan cPlan;
	fftwf_plan DeltaXPlan;
	fftwf_plan DeltaYPlan;
	float a_global; // phillips constant
	float lambda;
	float windSpeed;
	float windDir;
};
