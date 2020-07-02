/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_D3D9_VERTEX_FACTORY_H_
#define _NEXUS_D3D9_VERTEX_FACTORY_H_
#include "vertex_factory_type.h"
#include "../nengine/framework/view_info.h"

namespace nexus
{
	d3d_vb_ptr create_d3d_vb_from_stream(const vertex_stream* stream, D3DPOOL pool, bool init_copy);		

	class d3d9_vertex_factory
		: public nvertex_factory
	{
	public:
		typedef shared_ptr<d3d9_vertex_factory> ptr;

		d3d9_vertex_factory(void);
		virtual ~d3d9_vertex_factory(void);

		virtual void destroy();
		virtual void draw_set_pos_only() = 0;
		virtual bool draw_set_full();
		virtual void draw_reset()	{};

		virtual vertex_factory_type* get_type()	{	return m_type; }
		virtual vertex_factory_type* get_inst_type()	{	return m_inst_type; }
		size_t get_num_vert() const		{	return m_num_vert;}
		const vector<d3d_vb_ptr>& get_vert_streams()		{ return m_vert_streams; }
		const vector<UINT>& get_stream_stride()		{ return m_stream_stride; }
	
	protected:
		void draw_set_pos_only_static();

	protected:
		vector<d3d_vb_ptr>		m_vert_streams;
		vector<UINT>			m_stream_stride;
		vertex_factory_type*	m_type;
		vertex_factory_type* m_inst_type;
		size_t					m_num_vert;
		
	};
}//namespace nexus
#endif //_NEXUS_D3D9_VERTEX_FACTORY_H_