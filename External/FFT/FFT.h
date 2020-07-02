//-----------------------------------------------------------------------------
// File: fft.h
//
// Wrapper functions for the fftw3 library
//
// Copyright (c) 2006 ATI Technologies Inc. All rights reserved.
//-----------------------------------------------------------------------------
#ifndef _FFT_H_
#define _FFT_H_
#ifdef FFT_EXPORTS
#define FFT_API __declspec(dllexport)
#else
#define FFT_API __declspec(dllimport)
#endif

#include "fftw3.h"

// inverse fast forier transform
FFT_API fftwf_plan ifft_plan(int resolution, fftwf_complex *frequencyData, fftwf_complex *spatialData);
FFT_API void fft_execute(fftwf_plan plan);
FFT_API void destroy_plan(fftwf_plan plan);

#endif