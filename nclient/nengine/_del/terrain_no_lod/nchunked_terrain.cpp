#include "StdAfx.h"
#include <sstream>
#include "nchunked_terrain.h"
#include "nchunked_terrain_impl.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nchunked_terrain, nactor)

	nchunked_terrain::nchunked_terrain(const nstring& name_str)
		: nactor(name_str),m_impl(nNew nchunked_terrain_impl)
	{
	}

	nchunked_terrain::~nchunked_terrain(void)
	{
		delete m_impl;
	}

	void nchunked_terrain::create(size_t w, size_t h, short init_h, size_t chunk_size)
	{
		//-- create impl data
		m_impl->create(w, h, init_h, chunk_size);

		//-- create chunks
		size_t x_num = (w-1)/(chunk_size-1);
		size_t y_num = (h-1)/(chunk_size-1);
		size_t step = chunk_size-1;

		for(size_t y=0; y<y_num; y++)
		{
			for(size_t x=0; x<x_num; x++)
			{
				std::wostringstream ss;
				ss << _T("terrain_chunk_")
					<< x <<_T(":") << y;
				nterrain_chunk::ptr chunk_ptr = create_component_t<nterrain_chunk>(ss.str());
				chunk_ptr->create( npoint(x*step, y*step),
					m_impl);
			}
		}// end of for
	}

	void nchunked_terrain::destroy()
	{
		m_impl->destroy();
	}

	void nchunked_terrain::generate_noise(nrect rc, int numOctaves, float amplitude, float frequency)
	{
		m_impl->generate_noise(rc, numOctaves, amplitude, frequency);
	}

	void nchunked_terrain::set_scale(const vector3& scale)
	{
		m_scale = scale;

		matrix44 mat;
		mat_set_translation(mat, m_pos.x, m_pos.y, m_pos.z);
		mat_scale(mat, m_scale.x, m_scale.y, m_scale.z);
		move(mat);
	}

	void nchunked_terrain::set_position(const vector3& pos)
	{
		m_pos = pos;

		matrix44 mat;
		mat_set_translation(mat, m_pos.x, m_pos.y, m_pos.z);
		mat_scale(mat, m_scale.x, m_scale.y, m_scale.z);
		move(mat);
	}

	void nchunked_terrain::import_heightmap(const nstring& img_file_name)
	{
		m_impl->import_heightmap(img_file_name);
	}

	void nchunked_terrain::create_material_basic(const resource_location& texture_loc)
	{
		m_impl->create_material_basic(texture_loc);
	}
}//namespace nexus