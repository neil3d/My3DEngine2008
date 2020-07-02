#ifndef _NEXUS_EFFECT_VERTEX_DATA_H_
#define _NEXUS_EFFECT_VERTEX_DATA_H_
#include "../mesh/mesh_vertex_data.h"

namespace nexus
{
	/**
	 *	特效所使用的顶点数据
	 *	@remarks 位置，顶点色，贴图uv
	*/
	class neffect_vertex_data
		: public nmesh_vertex_data
	{
	public:
		neffect_vertex_data(void);
		virtual ~neffect_vertex_data(void);

		void init(size_t num_vert);

		//--
		virtual size_t get_num_stream() const	{	return 3;}
		virtual nstring get_vertex_type_name() const;
		virtual nstring get_vertex_factory_name() const;

		virtual vertex_stream* get_stream(size_t stream_index)
		{
			switch (stream_index)
			{
			case 0:
				return &m_pos_stream;
				break;
			case 1:
				return &m_diffuse_stream;
				break;
			case 2:
				return &m_uv_stream;
				break;
			default:
				nASSERT(0 && _T("Invalid stream index!"));
			}
			
			return NULL;
		}

		virtual void serialize(narchive& ar);

	private:
		vertex_stream	m_diffuse_stream;
		vertex_stream	m_uv_stream;
	};
}//namespace nexus

#endif //_NEXUS_EFFECT_VERTEX_DATA_H_