/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Aug, 2008
*/

#ifndef _NEXUS_VERTEX_ENUM_H_
#define _NEXUS_VERTEX_ENUM_H_

namespace nexus
{
	enum EVertexElementUsage
	{
		EVEU_None,
		EVEU_Position,
		EVEU_Normal,
		EVEU_Tangent,
		EVEU_TexCoord,
		EVEU_BlendIndices,
		EVEU_BlendWeight,
		EVEU_Color,
	};

	enum EVertexElementType
	{
		EVET_FLOAT1,
		EVET_FLOAT2,
		EVET_FLOAT3,
		EVET_FLOAT4,
		EVET_BYTE4,
	};

	struct vertex_element_define
	{
		unsigned short			stream_index;
		unsigned short			offset;
		EVertexElementType		type;
		EVertexElementUsage		usage;
		unsigned char			usage_index;
	};

	typedef vector<vertex_element_define> element_array;

	template<>
	inline narchive& nserialize(narchive& ar, vertex_element_define& v, const TCHAR* obj_name)
	{
		nstring class_name(_T("vertex_element_define"));
		ar.object_begin(obj_name, class_name);
			nserialize(ar, v.stream_index, _T("stream_index"));
			nserialize(ar, v.offset, _T("offset"));
			nserialize_enum_as_int(ar, v.type, _T("type"));
			nserialize_enum_as_int(ar, v.usage, _T("usage"));
			nserialize(ar, v.usage_index, _T("usage_index"));
		ar.object_end();
		return ar;
	}

	inline unsigned short get_element_size(enum EVertexElementType et)
	{
		switch(et)
		{
		case EVET_FLOAT1:
			return sizeof(float);
			break;
		case EVET_FLOAT2:
			return sizeof(float)*2;
			break;
		case EVET_FLOAT3:
			return sizeof(float)*3;
			break;
		case EVET_FLOAT4:
			return sizeof(float)*4;
			break;
		case EVET_BYTE4:
			return sizeof(unsigned char)*4;
			break;
		default:
			nASSERT(0 && "unknown vertex element type");
		}
		return 0;
	}

}//namespace nexus

#endif //_NEXUS_VERTEX_ENUM_H_