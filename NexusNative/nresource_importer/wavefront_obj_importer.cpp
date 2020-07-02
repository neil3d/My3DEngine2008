#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <limits>
#include "wavefront_obj_importer.h"

namespace nexus
{
	nDEFINE_CLASS(wavefront_obj_importer, nstatic_mesh_importer)

	wavefront_obj_importer::wavefront_obj_importer(void)
	{}

	wavefront_obj_importer::~wavefront_obj_importer(void)
	{}

	nstatic_mesh_lod::ptr wavefront_obj_importer::import_from_file(const nstring& full_path)
	{
		wfstream in_file( full_path.c_str(), std::ios_base::in );

		if( !in_file )
			nthrow2(_T("file open failed."), full_path.c_str());

		nstatic_mesh_lod::ptr ret(nNew nstatic_mesh_lod);		

		std::vector<vector3>	pos_array;
		std::vector<vector2>	tex_array;
		std::vector<vector3>	normal_array;

		nmesh_section::ptr	cur_section;		

		size_t total_vert = 0;
		size_t total_face = 0;
		size_t total_section = 0;

		nstring cmd;
		while( in_file )
		{
			in_file >> cmd;

			if( cmd == _T("#"))			// comment
			{
			}
			else if( cmd == _T("g"))
			{
			}
			else if( cmd == _T("v"))	// vertex position
			{
				float x, y, z;
				in_file >> x >> y >> z;

				pos_array.push_back(vector3(x,y,-z));
			}
			else if( cmd == _T("vt"))	// vertex textrue coord
			{
				float u, v;
				in_file >> u >> v;

				tex_array.push_back(vector2(u, 1-v));	// convert to D3D style texcoord
			}
			else if( cmd == _T("vn"))	// vertex normal
			{
				float x, y, z;
				in_file >> x >> y >> z;

				normal_array.push_back(vector3(x,y,-z));
			}
			else if( cmd == _T("f"))	// face
			{
				int pos_index=-1,
					tex_index=-1, 
					normal_index=-1;

				// triangle only
			
				vertex_traits  traits[3];
				vertex vert[3];
				for(int i=0; i<3; i++)
				{
					// OBJ format uses 1-based arrays
					in_file >> pos_index;
					vert[i].pos = pos_array[pos_index-1];

					if( '/' == in_file.peek() )
					{
						in_file.ignore();

						if( '/' != in_file.peek() )
						{
							// Optional texture coordinate
							in_file >> tex_index;
							vert[i].tex = tex_array[ tex_index-1 ];
						}

						if( '/' == in_file.peek() )
						{
							in_file.ignore();

							// Optional vert normal
							in_file >> normal_index;
							vert[i].normal = normal_array[ normal_index-1 ];
						}
					}

					traits[i].pos_index = pos_index;
					traits[i].normal_index = normal_index;
					traits[i].tex_index = tex_index;

					if(!cur_section) //  必须在face命令前收到usemtl命令，否则cur_section未创建
					{
						cur_section.reset(nNew nmesh_section);
						cur_section->m_primitive_type = EDraw_TriangleList;		
						cur_section->m_material_id = ret->m_secton_array.size();
					}
				}

				cur_section->m_index_buffer.append_index(add_vertex(traits[0],vert[0]));
				cur_section->m_index_buffer.append_index(add_vertex(traits[2],vert[2]));
				cur_section->m_index_buffer.append_index(add_vertex(traits[1],vert[1]));
				//endof for()
				total_face++;
			}
			else if( cmd == _T("usemtl"))	// 根据材质切分section
			{
				nstring mtl_name;
				in_file >> mtl_name;

				int mtl_id = get_material_index(mtl_name);			

				if( mtl_id == -1)
				{
					m_material_lib.push_back(mtl_name);
					mtl_id = m_material_lib.size()-1;

					add_mesh_section(ret, cur_section);					
					cur_section.reset(nNew nmesh_section);
					cur_section->m_primitive_type = EDraw_TriangleList;		
					cur_section->m_material_id = mtl_id;
				}
				else
				{
					bool find = false;
					// 找到使用同样材质的section，继续向里面添加face
					for(size_t i=0; i<ret->m_secton_array.size(); i++)
					{
						if( ret->m_secton_array[i]->m_material_id == mtl_id)
						{
							add_mesh_section(ret, cur_section);
							cur_section = ret->m_secton_array[i];							
							find = true;
							break;
						}
					}

					nASSERT(find); //  前面肯定已经建立了相应的section
				}
			}
			else
			{
				// unknown command
			}

			in_file.ignore( 1024, '\n' );
		}

		if(cur_section->m_index_buffer.get_data_size() > 0)
		{
			add_mesh_section(ret, cur_section);			
		}

		//--
		generate_tangent(ret);
		
		//--
		ret->m_bounding_box = compute_bounding_box();

		//-- create nmesh_vertex_data
		ret->m_vert_data = create_vertex_data();

		total_section = ret->m_secton_array.size();
		total_vert = m_vertex_buffer.size();

		nLog_Info(_T("%s imported, total vert=%d, total section = %d, total face = %d\r\n"), 
			full_path.c_str(), total_vert, total_section, total_face);

		return ret;
	}

	unsigned short wavefront_obj_importer::add_vertex(const vertex_traits& traits, const vertex& vt)
	{
		std::map<vertex_traits, size_t>::iterator iter = m_traits2index.find(traits);
		if(iter != m_traits2index.end())
			return iter->second;

		m_vertex_buffer.push_back(vt);
		unsigned short new_index = (unsigned short)(m_vertex_buffer.size()-1);

		m_traits2index.insert(make_pair(traits, new_index));
		return new_index;
	}

	int wavefront_obj_importer::get_material_index(const nstring& mtl_name)
	{
		for(size_t i=0; i< m_material_lib.size(); i++)
		{
			if( m_material_lib[i] == mtl_name)
				return (int)i;
		}

		return -1;
	}

	void wavefront_obj_importer::add_mesh_section(nstatic_mesh_lod::ptr lod, nmesh_section::ptr sec)
	{
		if( !sec )
			return;
		if( sec->m_index_buffer.get_index_count() <= 0)
			return;

		for(size_t i=0; i< lod->m_secton_array.size(); i++)
		{
			if( sec == lod->m_secton_array[i])
				return;
		}

		lod->m_secton_array.push_back(sec);
	}

}//namespace nexus