//======================================================================
//
//	COPY FROM D3D SAMPLE --  HDR Pipeline
//
//======================================================================

texture g_texLum;
texture g_texLastLum;

sampler s0 = sampler_state 
{
    Texture   = <g_texLum>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;    
};

sampler s1 = sampler_state 
{
    Texture   = <g_texLastLum>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;    
};


//======================================================================
//
//      HIGH DYNAMIC RANGE RENDERING DEMONSTRATION
//      Written by Jack Hoxley, November 2005
//
//======================================================================



//------------------------------------------------------------------
//  GLOBAL VARIABLES
//------------------------------------------------------------------
float4      tcLumOffsets[4];                // The offsets used by GreyScaleDownSample()
float4      tcDSOffsets[9];                 // The offsets used by DownSample()


//------------------------------------------------------------------
//  This entry point performs the basic first-pass when measuring
//  luminance of the HDR render target. It samples the HDR target
//  multiple times so as to compensate for the down-sampling and
//  subsequent loss of data.
//------------------------------------------------------------------
float4 GreyScaleDownSample( in float2 t : TEXCOORD0 ) : COLOR0
{

    // Compute the average of the 4 necessary samples
        float average = 0.0f;
        float maximum = -1e20;
        float4 color = 0.0f;
        float3 WEIGHT = float3( 0.299f, 0.587f, 0.114f );
        
        for( int i = 0; i < 4; i++ )
        {
            color = tex2D( s0, t + float2( tcLumOffsets[i].x, tcLumOffsets[i].y ) );
            
            // There are a number of ways we can try and convert the RGB value into
            // a single luminance value:
            
            // 1. Do a very simple mathematical average:
            //float GreyValue = dot( color.rgb, float3( 0.33f, 0.33f, 0.33f ) );
            
            // 2. Perform a more accurately weighted average:
            //float GreyValue = dot( color.rgb, WEIGHT );
            
            // 3. Take the maximum value of the incoming, same as computing the
            //    brightness/value for an HSV/HSB conversion:
            float GreyValue = max( color.r, max( color.g, color.b ) );
            
            // 4. Compute the luminance component as per the HSL colour space:
            //float GreyValue = 0.5f * ( max( color.r, max( color.g, color.b ) ) + min( color.r, min( color.g, color.b ) ) );
            
            // 5. Use the magnitude of the colour
            //float GreyValue = length( color.rgb );
                        
            maximum = max( maximum, GreyValue );
            average += (0.25f * log( 1e-5 + GreyValue )); //1e-5 necessary to stop the singularity at GreyValue=0
        }
        
        average = exp( average );
        
    // Output the luminance to the render target
        return float4( average, maximum, 0.0f, 1.0f );
        
}
    
    
    
//------------------------------------------------------------------
//  This entry point will, using a 3x3 set of reads will downsample
//  from one luminance map to another.
//------------------------------------------------------------------
float4 DownSample( in float2 t : TEXCOORD0 ) : COLOR0
{
    
    // Compute the average of the 10 necessary samples
        float4 color = 0.0f;
        float maximum = -1e20;
        float average = 0.0f;
        
        for( int i = 0; i < 9; i++ )
        {
            color = tex2D( s0, t + float2( tcDSOffsets[i].x, tcDSOffsets[i].y ) );
            average += color.r;
            maximum = max( maximum, color.g );
        }
        
    // We've just taken 9 samples from the
    // high resolution texture, so compute the
    // actual average that is written to the
    // lower resolution texture (render target).
        average /= 9.0f;
        
    // Return the new average luminance
        return float4( average, maximum, 0.0f, 1.0f );
}

technique lumFirstPass
{
	pass p0
	{
		VertexShader = NULL;
		PixelShader = compile ps_2_0 GreyScaleDownSample();
	}
}

technique lumDownSample
{
	pass p0
	{
		VertexShader = NULL;
		PixelShader = compile ps_2_0 DownSample();
	}
}

// Copy from HDRLighting sample in D3D SDK
float  g_fElapsedTime;      // Time in seconds since the last calculation

//-----------------------------------------------------------------------------
// Name: CalculateAdaptedLum
// Type: Pixel shader                                      
// Desc: Calculate the luminance that the camera is current adapted to, using
//       the most recented adaptation level, the current scene luminance, and
//       the time elapsed since last calculated
//-----------------------------------------------------------------------------
float4 CalculateAdaptedLum
    (
    in float2 vScreenPosition : TEXCOORD0
    ) : COLOR
{
    float fAdaptedLum = tex2D(s0, float2(0.5f, 0.5f));
    float fCurrentLum = tex2D(s1, float2(0.5f, 0.5f));
    
    // The user's adapted luminance level is simulated by closing the gap between
    // adapted luminance and current luminance by 2% every frame, based on a
    // 30 fps rate. This is not an accurate model of human adaptation, which can
    // take longer than half an hour.
    float fNewAdaptation = fAdaptedLum + (fCurrentLum - fAdaptedLum) * ( 1 - pow( 0.98f, 30 * g_fElapsedTime ) );
    return float4(fNewAdaptation, fNewAdaptation, fNewAdaptation, 1.0f);
}

technique lumAdapted
{
	pass p0
	{
		VertexShader = NULL;
		PixelShader = compile ps_2_0 CalculateAdaptedLum();
	}
}