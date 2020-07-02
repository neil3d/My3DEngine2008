#ifndef _NEXUS_IMAGE_DEFINE_H_
#define _NEXUS_IMAGE_DEFINE_H_

namespace nexus
{
	enum EPixelFormat
	{
		EPF_None,
		EPF_A8R8G8B8,	// 32 bits
		EPF_R8G8B8,		// 24 bits
	};

	struct pixle_A8R8G8B8
	{
		unsigned char A,R,G,B;
	};

	struct pixel_R8G8B8
	{
		unsigned char R,G,B;
	};
}

#endif //_NEXUS_IMAGE_DEFINE_H_