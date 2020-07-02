/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _RENDER_DEFINE_H_
#define _RENDER_DEFINE_H_

namespace nexus
{
	typedef unsigned int hit_id;

	enum ERenderMode
	{
		ERM_Wireframe,
		ERM_Unlit,
		ERM_Lit,
		ERM_LightingOnly,
	};

	enum EDepthGroup
	{
		EDG_Background = 0,	// 背景组
		EDG_World,			// 世界对象组
		EDG_Foreground,		// 前景组
		EDG_Max,
	};

	enum ETransparentType
	{
		ETrans_Opaque= 0,		// 完全不透明
		ETrans_AlphaMasked,		// 不透明，但是需要alpha mask
		ETrans_Mixed,			// 物体有半透明的部分, 也有不透明的部分		
		ETrans_UnlitTrans,		// 完全不透明，也不需要光照
		ETrans_Max
	};

	struct render_config
	{
		void*	window_handle;
		size_t	width,
				height;
		size_t	color_bits;		
		bool	bWindowed;
		bool	bEnableHDR;
	};

	enum EPrimitiveType
	{
		EDraw_None = 0,
		EDraw_PointList,
		EDraw_LineList,
		EDraw_LineStrip,
		EDraw_TriangleList,
		EDraw_TriangleStrip,
		EDraw_TriangleFan,
	};

}//namespace nexus

#endif //_RENDER_DEFINE_H_