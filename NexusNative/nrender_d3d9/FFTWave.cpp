#include "StdAfx.h"
#include "FFTWave.h"
#include <time.h>
#include <sys/types.h>	// needed to use _timeb
#include <sys/timeb.h>	// needed to use _timeb

inline float neg1Pow(int k)
{
	return pow(-1.0f,k);
}

float Phillips(float A,float kx, float ky, float windSpeed, float windDirection)
{
	float fWindDir = windDirection * D3DX_PI / 180.0f;  // Convert wind direction from global degrees

	float L = windSpeed * windSpeed / 9.81f;      // largest possible wave from constant wind of velocity 
	float w = L / 75;                             // damp out waves with very small length w << L

	float ksqr = kx * kx + ky * ky;
	float kdotwhat = (float) (kx * cos(fWindDir) + ky * sin(fWindDir));
	kdotwhat = max(0.0f, kdotwhat);
	float phillips = (float) (A * (pow(2.7183f, -1.0f / (L * L * ksqr)) * (kdotwhat * kdotwhat)) / (ksqr * ksqr * ksqr));  // Equation (23) in [Tessendorf99]
	float damp = (float) exp(-ksqr * w * w);                                                                               // Equation (24) in [Tessendorf99]

	if (kdotwhat < 0)
	{
		phillips *= 0.25f;  // filter out waves moving opposite to wind
	}

	return (phillips * damp);
}

//参数为FFT模拟的周期
FFTWave::FFTWave(float cycle_length)
{
	a_global=0.000001f; // phillips constant
	lambda = 0.5;
	windSpeed = 25;
	windDir=30;
	WorldSizeX = cycle_length;
	WorldSizeY = cycle_length;

	cPlan = ifft_plan( NX, (fftwf_complex *)c,(fftwf_complex *)c );

	DeltaXPlan = ifft_plan( NX, (fftwf_complex *)mDeltaX,(fftwf_complex *)mDeltaX );
	DeltaYPlan = ifft_plan( NY, (fftwf_complex *)mDeltaY,(fftwf_complex *)mDeltaY );

	calculate_ho();
}

FFTWave::~FFTWave(void)
{
	destroy_plan(cPlan);
	destroy_plan(DeltaXPlan);
	destroy_plan(DeltaYPlan);
}

static float normalDistTable[][2] = {
	{.00f, .5f}, {.01f, .504f}, {.02f, .508f}, {.03f, .512f}, {.06f, .5239f}, {.08f, .5319f}, {.11f, .5438f}, {.13f, .5517f},
	{.16f, .5636f}, {.18f, .5714f}, {.21f, .5832f}, {.23f, .5910f}, {.26f, .6026f}, {.28f, .6103f}, {.31f, .6217f}, {.34f, .6331f},
	{.36f, .6406f}, {.39f, .6517f}, {.42f, .6628f}, {.44f, .6700f}, {.47f, .6808f}, {.50f, .6915f}, {.53f, .7019f}, {.56f, .7123f},
	{.59f, .7224f}, {.62f, .7324f}, {.65f, .7422f}, {.68f, .7517f}, {.71f, .7611f}, {.74f, .7703f}, {.78f, .7823f}, {.81f, .7910f},
	{.85f, .8023f}, {.88f, .8106f}, {.92f, .8212f}, {.96f, .8315f}, {1.0f, .8413f}, {1.04f, .8508f}, {1.09f, .8621f}, {1.13f, .8708f},
	{1.18f, .8810f}, {1.23f, .8907f}, {1.29f, .9015f}, {1.35f, .9115f}, {1.41f, .9207f}, {1.48f, .9306f}, {1.56f, .9406f},
	{1.65f, .9505f}, {1.76f, .9608f}, {1.89f, .9706f}, {2.06f, .9803f}, {2.33f, .9901f}, {99.0f, 1.0f}
};

float RandNormal(float mean, float stdev)
{
	int i = 0;
	float u = rand() / (float) RAND_MAX;
	float n;

	if (u >= 0.5)
	{
		while (u > normalDistTable[i][1])
		{
			i++;
		}
		n = normalDistTable[i-1][0];
	}
	else
	{
		u = 1 - u;
		while (u > normalDistTable[i][1])
		{
			i++;
		}
		n = 1 - normalDistTable[i-1][0];
	}

	//printf("u: %f, n: %f, i: %d\n", u, n, i); //debug

	return (mean + stdev * n);
}

void FFTWave::calculate_ho()
{
	int i, j;
	float kx, ky;
	float fMultiplier, fAmplitude, fTheta;

	// Check to see if this is the first time through this code
	static bool bFirstTimeThrough = TRUE;

	//
	// Fill up the low frequency arrays which will get copied into textures later
	//
	for (i = 0; i < NX; i++)
	{
		kx = (-((float)NY) / 2.0f + ((float)i)) * (2.0f * D3DX_PI / WorldSizeX);

		for (j = 0; j < NY; j++)
		{
			ky = (-((float)NY) / 2.0f + ((float)j)) * (2.0f * D3DX_PI / WorldSizeY);

			if ((kx == 0.0f) && (ky == 0.0f))
				fMultiplier = 0.0f;
			else
				fMultiplier = sqrt(Phillips(a_global,kx, ky, windSpeed, windDir));

			fAmplitude = RandNormal(0.0f, 1.0f);
			fTheta = rand() / (float) RAND_MAX * 2 * D3DX_PI;

			mH0[i][j].real = fMultiplier * fAmplitude * sin(fTheta);
			mH0[i][j].imag = fMultiplier * fAmplitude * sin(fTheta);

			hold_horizontal[i][j][0] = kx;
			hold_horizontal[i][j][1] = ky;
			hold_horizontal[i][j][3]=hold_horizontal[i][j][0]*hold_horizontal[i][j][0]+hold_horizontal[i][j][1]*hold_horizontal[i][j][1];
			hold_horizontal[i][j][2]=sqrt(hold_horizontal[i][j][3]);
		}
	}
}

void FFTWave::prep_loop()
{
	for (int i=0;i<NX;i++)
	{
		for (int j=0;j<NY;j++)
		{
			sea[i][j][0]=displayXY[i][j][0];
			sea[i][j][1]=displayXY[i][j][1];
			sea[i][j][2]=c[i][j].real;
		}
	}
}

void FFTWave::pre_choppy()
{
	//this function sets up the DX DY choppiness
	// it assumes that the current c values are in position
	// before the c values have been FFT'd

	float k[2],klen;

	for (int i=0;i<NX;i++)
	{
		for (int j=0;j<NY;j++)
		{
			k[0]=hold_horizontal[i][j][0];
			k[1]=hold_horizontal[i][j][1];

			klen=hold_horizontal[i][j][2];
			if (klen==0)
			{
				mDeltaX[i][j].real=0.0;
				mDeltaX[i][j].imag=0.0;
				mDeltaY[i][j].real=0.0;
				mDeltaY[i][j].imag=0.0;
			}
			else
			{
				mDeltaX[i][j].real=0.0;
				mDeltaX[i][j].imag=c[i][j].imag*(-k[0]/klen);
				mDeltaY[i][j].real=0.0;
				mDeltaY[i][j].imag=c[i][j].imag*(-k[1]/klen);
			}
		}
	}

	fft_execute(DeltaXPlan);
	fft_execute(DeltaYPlan);

	for (int i=0;i<NX;i++)
	{
		for (int j=0;j<NY;j++)
		{
			mDeltaX[i][j].real *= neg1Pow(i+j)*lambda;
			mDeltaX[i][j].imag *= neg1Pow(i+j)*lambda;
			mDeltaY[i][j].real *= neg1Pow(i+j)*lambda;
			mDeltaY[i][j].imag *= neg1Pow(i+j)*lambda;
		}
	}
}

void FFTWave::idle(float deltaTime)
{
	float kvector[2],klength,wkt;

	//////////////////alernativ below///////////
	//int yHalf = NY/2 + 1;
	for (int i = 0; i<NY; ++i)
	{
		//int yLine = i*NY;
		// Mirror the y line index for calculation of -k
		// The line below evalutes yLineMirr = y == 0 ? 0 : (mYSize-y)*mXSize;
		// by wrapping the heightmap, since it is periodic.
		//int yLineMirr = ((NY-i)% NY)*NY;
		for (int j = 0; j<NX; ++j)
		{
			kvector[0]=hold_horizontal[i][j][0];
			kvector[1]=hold_horizontal[i][j][1];

			klength=hold_horizontal[i][j][2];

			wkt = sqrt(klength * GRAV_CONSTANT) * deltaTime;

			//int  kNegIndex = yLineMirr*NY + ((NY-j)% NY);

			// This is h~(K, t) from the Tessendorf paper.
			c[i][j].real= mH0[i][j].real*cos(wkt)
				+ mH0[i][j].imag*sin(wkt)
				+ mH0[NX - i-1][NY - j-1].real*cos(wkt)
				- mH0[NX - i-1][NY -j-1].imag*sin(wkt);

			c[i][j].imag= mH0[i][j].imag*cos(wkt)
				+ mH0[i][j].real*sin(wkt)
				-mH0[NX - i-1][NY - j-1].imag*cos(wkt)
				- mH0[NX - i-1][NY -j-1].real*sin(wkt);
		}
	}

	//////////////////end alternative////////////
	pre_choppy();
	fft_execute(cPlan);
	
	///////////////negative power term creation
	for (int i=0;i<NX;i++)
	{
		for (int j=0;j<NY;j++)
		{
			c[i][j].real *= float(neg1Pow(i+j));

			// while we are looping, set up the final x,y values for display
			displayXY[i][j][0]=((float)i/NX)*WorldSizeX+mDeltaX[i][j].imag;
			displayXY[i][j][1]=((float)j/NY)*WorldSizeY+mDeltaY[i][j].imag;
		}
	}

	prep_loop();	//this loop loads the actual sea vertices
}
