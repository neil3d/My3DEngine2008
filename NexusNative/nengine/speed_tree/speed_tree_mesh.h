#ifndef _NEXUS_SPEED_TREE_MESH_H_
#define _NEXUS_SPEED_TREE_MESH_H_
#include "spt_cfg.h"
#include "../mesh/mesh_vertex_data.h"
#include "../mesh/mesh_index_data.h"

namespace nexus
{
	class nspt_vertex_data
		: public nmesh_vertex_data
	{
	public:
		nspt_vertex_data(void)	{}
		virtual ~nspt_vertex_data(void)	{}

		virtual size_t get_num_stream() const	{	return 2;}
		virtual vertex_stream* get_stream(size_t stream_index)
		{
			switch(stream_index)
			{
			case 0:		return &m_pos_stream;		break;
			case 1:		return &m_packed;			break;			
			default:	nASSERT(0 && "bad stream index");	break;
			}
			return NULL;
		}

		virtual bool support_pos_only() const	{	return false;}

	protected:		
		vertex_stream	m_packed;
	};

	/**
	 *	SpeedTree branch vertex data
	*/
	class nspt_branch_vertex_data
		: public nspt_vertex_data
	{
	public:
		nspt_branch_vertex_data(void)	{}
		virtual ~nspt_branch_vertex_data(void)	{}

		virtual nstring get_vertex_type_name() const;
		virtual nstring get_vertex_factory_name() const;		

		int create(const CSpeedTreeRT::SGeometry& spt);
		int get_triangle_strip(const CSpeedTreeRT::SGeometry& spt, int nLod, index_buffer16& indices);
	};

	/**
	 *	SpeedTree frond vertex data
	*/
	class nspt_frond_vertex_data
		: public nspt_vertex_data
	{
	public:
		nspt_frond_vertex_data(void)	{}
		virtual ~nspt_frond_vertex_data(void)	{}

		virtual nstring get_vertex_type_name() const;
		virtual nstring get_vertex_factory_name() const;		

		int create(const CSpeedTreeRT::SGeometry& spt);
		int get_triangle_strip(const CSpeedTreeRT::SGeometry& spt, int nLod, index_buffer16& indices);
	};

	class nspt_leafcard_vertex_data
		: public nspt_vertex_data
	{
	public:
		nspt_leafcard_vertex_data(void)	{}
		virtual ~nspt_leafcard_vertex_data(void)	{}

		virtual nstring get_vertex_type_name() const;
		virtual nstring get_vertex_factory_name() const;		

		int create(const CSpeedTreeRT::SGeometry::SLeaf& sLeaves);		
	};

	class nspt_leafmesh_vertex_data
		: public nspt_vertex_data
	{
	public:
		nspt_leafmesh_vertex_data(void)	{}
		virtual ~nspt_leafmesh_vertex_data(void)	{}

		virtual nstring get_vertex_type_name() const;
		virtual nstring get_vertex_factory_name() const;		

		int create(const CSpeedTreeRT::SGeometry::SLeaf& sLeaves, index_buffer16& indices);		
	};
}

#endif //_NEXUS_SPEED_TREE_MESH_H_