/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_MESH_FUNC_H_
#define _NEXUS_MESH_FUNC_H_

namespace nexus
{
	inline size_t get_primitive_count_by_vert(enum EPrimitiveType draw_type,size_t num_vert)
	{
		size_t ret=0;
		switch(draw_type)
		{
		case EDraw_PointList:
			ret=num_vert;
			break;
		case EDraw_LineList:
			ret=num_vert/2;
			break;
		case EDraw_LineStrip:
			ret=num_vert-1;
			break;
		case EDraw_TriangleList:
			ret=num_vert/3;
			break;
		case EDraw_TriangleStrip:
			ret=num_vert-2;
			break;
		case EDraw_TriangleFan:
			ret=num_vert-2;
			break;
		default:
			nASSERT(0 && "unknown draw type");
		}
		return ret;
	}

	inline D3DPRIMITIVETYPE conv_primitive_type(enum EPrimitiveType pm_type)
	{
		switch(pm_type)
		{
		case EDraw_PointList:		 return D3DPT_POINTLIST;
		case EDraw_LineList:		 return D3DPT_LINELIST;
		case EDraw_LineStrip:		 return D3DPT_LINESTRIP;
		case EDraw_TriangleList:	 return D3DPT_TRIANGLELIST;
		case EDraw_TriangleStrip:	 return D3DPT_TRIANGLESTRIP;
		case EDraw_TriangleFan:		 return D3DPT_TRIANGLEFAN;
		}
		return D3DPT_FORCE_DWORD;
	}

}//namespace nexus

#endif //_NEXUS_MESH_FUNC_H_