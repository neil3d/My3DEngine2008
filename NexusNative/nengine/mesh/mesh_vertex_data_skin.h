#ifndef _NEXUS_MESH_VERTEX_DATA_SKIN_H_
#define _NEXUS_MESH_VERTEX_DATA_SKIN_H_
#include "mesh_vertex_data.h"

namespace nexus
{
	const size_t SKIN_MAX_INFLUENCES = 4;
	/**
	 *	skeletal mesh使用的vertex data
	 *	@remark 只支持一重uv；支持normal，可选择是否包含tangent
	*/
	class nAPI nmesh_vertex_data_skin
		: public nmesh_vertex_data
	{
	public:
		typedef shared_ptr<nmesh_vertex_data_skin> ptr;

		nmesh_vertex_data_skin(void);
		~nmesh_vertex_data_skin(void);

		void init(size_t num_vert, bool have_tangent);	

		virtual nstring get_vertex_type_name() const;
		virtual nstring get_vertex_factory_name() const;

		virtual size_t get_num_stream() const	{	return 2;}
		virtual vertex_stream* get_stream(size_t i)
		{
			nASSERT(i < 2);
			vertex_stream* ptr_array[3] = 
			{	&m_pos_stream,
				&m_second_stream,	
				0,
			};

			return ptr_array[i];
		}

		bool has_tangent() const	{	return m_tangent; }

		virtual void serialize(narchive& ar)
		{
			nmesh_vertex_data::serialize(ar);
			nSERIALIZE(ar, m_tangent);
			nSERIALIZE(ar, m_second_stream);
		}
	protected:
		bool	m_tangent;

		/**
			struct vert_part
			{
				vector3		normal;
				[vector3	tangent;]
				vector2		uv;
				byte		bone_index[4];
				float		bone_weight[4];
			};
		*/
		vertex_stream	m_second_stream;

		nDECLARE_CLASS(nmesh_vertex_data_skin)
	};
}//namespace nexus
#endif //_NEXUS_MESH_VERTEX_DATA_SKIN_H_