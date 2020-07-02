/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_MESH_VERTEX_DATA_H_
#define _NEXUS_MESH_VERTEX_DATA_H_
#include "ncore.h"
#include "vertex_stream.h"

namespace nexus
{
	struct shader_define
	{
		std::string	name;
		std::string	value;

		shader_define(void)	{}
		shader_define(const std::string& _name, const std::string& _value):name(_name),value(_value)
		{}
	};
	typedef std::vector<shader_define> shader_macro_array;

	template<>
	inline narchive& nserialize(narchive& ar, shader_define& d, const TCHAR* obj_name)
	{
		nstring class_name(_T("shader_define"));
		ar.object_begin(obj_name, class_name);
			nserialize(ar, d.name, _T("name"));
			nserialize(ar, d.value, _T("value"));		
		ar.object_end();
		return ar;
	}

	/**
	 *	管理一个Mesh所需的vertex相关数据
	*/
	class nAPI nmesh_vertex_data
		: public nobject
	{
	public:
		typedef shared_ptr<nmesh_vertex_data> ptr;

		nmesh_vertex_data(void);
		virtual ~nmesh_vertex_data(void);

		virtual size_t get_num_stream() const = 0;
		virtual vertex_stream* get_stream(size_t stream_index) = 0;

		virtual nstring get_vertex_type_name() const = 0;		//	对应不同的顶点格式－－vertex declaration
		virtual nstring get_vertex_factory_name() const = 0;	//	对应不同的factory shader file
		
		virtual const shader_macro_array& get_macro_array() const	{	return m_macro_array;}		
		size_t get_stream_data_size(size_t stream_index) const	{	return const_cast<nmesh_vertex_data*>(this)->get_stream(stream_index)->get_data_size();}
		void* get_stream_data(size_t stream_index) const		{	return const_cast<nmesh_vertex_data*>(this)->get_stream(stream_index)->get_data();}
		size_t get_stream_stride(size_t stream_index) const
		{
			return const_cast<nmesh_vertex_data*>(this)->get_stream(stream_index)->get_vertex_size();
		}
		const element_array& get_stream_elements_define(size_t stream_index) const {	return const_cast<nmesh_vertex_data*>(this)->get_stream(stream_index)->get_elemets_define();}
		size_t get_num_vert() const		{	return const_cast<nmesh_vertex_data*>(this)->get_stream(0)->get_num_vert();}		

		virtual void serialize(narchive& ar);
	protected:
		void init_pos_stream(size_t num_vert);
		vertex_stream	m_pos_stream;		//	保存position

		shader_macro_array	m_macro_array;

		nDECLARE_VIRTUAL_CLASS(nmesh_vertex_data)
	};

	/**
	 *	包含一个pos,一个normal两个stream的顶点数据
	*/
	class nmesh_vertex_data_pos_normal
		: public nmesh_vertex_data
	{
	public:
		nmesh_vertex_data_pos_normal(void);
		virtual ~nmesh_vertex_data_pos_normal(void);

		virtual size_t get_num_stream() const	{	return 2;}
		virtual nstring get_vertex_type_name() const;
		virtual nstring get_vertex_factory_name() const;
		
		void init(size_t num_vert);
		vertex_stream& get_pos_stream()	{	return m_pos_stream;}
		vertex_stream& get_normal_stream()	{	return m_normal_stream;}

		virtual vertex_stream* get_stream(size_t stream_index)
		{
			if(stream_index == 0)
				return &m_pos_stream;
			
			nASSERT(stream_index == 1);
			return &m_normal_stream;
		}

		virtual void serialize(narchive& ar);
	private:
		vertex_stream	m_normal_stream;

		nDECLARE_CLASS(nmesh_vertex_data_pos_normal)
	};
}//namespace nexus
#endif //_NEXUS_MESH_VERTEX_DATA_H_