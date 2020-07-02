#ifndef _NEXUS_RENDER_STATE_H_
#define _NEXUS_RENDER_STATE_H_

namespace nexus
{
	enum ECmpFunc
	{
		ECMP_Never = 1,	//same as D3D
		ECMP_Less,
		ECMP_Equal,
		ECMP_LessEqual,
		ECMP_Greater,
		ECMP_NotEqual,
		ECMP_GreaterEqual
	};

	enum EAlphaBlend
	{
		EAB_Zero = 1,
		EAB_One = 2,		
		EAB_SrcColor = 3,
		EAB_InvSrcColor = 4,
		EAB_SrcAlpha= 5,
		EAB_InvSrcAlpha = 6,
		EAB_DstAlpha = 7,
		EAB_InvDstAlpha = 8,
		EAB_DstColor = 9,
		EAB_InvDstColor = 10,
		EAB_SrcAlphaSat        = 11,
		EAB_BothSrcAlpha       = 12,
		EAB_BothInvSrcAlpha    = 13,
		EAB_BlendFactor        = 14, 
		EAB_InvBlendFactor     = 15
	};

	enum ECullMode
	{
		ECM_None = 1 ,//same as d3d
		ECM_CW,
		ECM_CCW,
	};

	enum EFillMode
	{
		EFM_Point = 1,
		EFM_Wireframe,
		EFM_Solid,
	};

	enum ERenderState
	{
		ERS_Begin = 0,
		ERS_ZTestEnable,	// true or false
		ERS_ZWriteEnable,	// true or false
		ERS_ZTestFunc,		// see enum ECmpFunc

		ERS_CullMode,		// see enum ECullMode
		ERS_FillMode,		// see enum EFillMode
		ERS_DepthBias,		// 0~16
		ERS_SlopeScaleDepthBias,

		ERS_AlphaTestEnable,// true or false
		ERS_AlphaRef,		// 0~255
		ERS_AlphaFunc,		// see enum ECmpFunc

		ERS_AlphaBlendEnable,		
		ERS_SrcBlend,		// enum EAlphaBlend
		ERS_DestBlend,		// enum EAlphaBlend

		ERS_End
	};

	struct default_render_state
	{
		int m_value[ERS_End];
	};
}//namespace nexus

#endif //_NEXUS_RENDER_STATE_H_