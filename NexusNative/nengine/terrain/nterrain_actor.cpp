#include "stdafx.h"
#include "nterrain_actor.h"
#include "../util/perlin.h"
#include "../image/nimage.h"
#include "../framework/nengine.h"
#include "../../ncore/height_map/height_map_util.h"
#include "../physics/nphys_util.h"
#include "nterrain_chunk.h"
#include "nterrain_deco_chunk_base.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nterrain_actor, nactor)

	nterrain_actor::nterrain_actor(const nstring& name_str)
		: nactor(name_str), m_chunk_size(65),m_phys_height_field(NULL)
	{
		m_frozen = true;
		m_depth_group = EDG_WorldGeometry;
	}

	nterrain_actor::~nterrain_actor(void)
	{
	}

	void nterrain_actor::create(size_t w, size_t h, unsigned short init_h, size_t chunk_size)
	{
		nASSERT(w > 0 && h > 0);
		nASSERT(chunk_size < 256);

		m_chunk_size = chunk_size;

		//-- clear old data
		m_mtl_setup.reset(this);
		m_deco_setup.reset(this);
		m_chunks.clear();

		//-- create maps
		m_height_map.create(w, h, init_h);
		m_viz_map.create(w, h, 1);

		//-- create chunk components
		create_chunks();
		
	}

	void nterrain_actor::create_chunks()
	{
		size_t w = m_height_map.get_width();
		size_t h = m_height_map.get_height();

		int yc = (h-1)/(m_chunk_size-1);
		int xc = (w-1)/(m_chunk_size-1);
		for(int y=0; y<yc; y++)
		{
			for(int x=0; x<xc; x++)
			{
				std::wostringstream ss;
				ss << _T("trn_chunk_") << x << _T('_') << y;

				nterrain_chunk::ptr ck = create_component_t<nterrain_chunk>( ss.str() );
				int px = x*(m_chunk_size-1);
				int py = y*(m_chunk_size-1);
				ck->create( px, py, m_chunk_size );

				//-- 虽然基类已经保存了components list，这里重复保存一次，方便处理
				m_chunks.push_back(ck);
			}
		}//end of for()

		m_deco_setup.create_chunks();
	}


	void nterrain_actor::generate_noise(nrect rc, int numOctaves, float amplitude, float frequency)
	{
		clip_rect(rc.left, rc.top, rc.right, rc.bottom, 
			m_height_map.get_width(), m_height_map.get_height());

		if( rc.get_width() <= 0
			|| rc.get_height() <= 0)
			return;

		Perlin pn(numOctaves, frequency, amplitude, rand());

		int ix,iy;
		float xStep = 1.0f/(rc.right-rc.left);
		float yStep = 1.0f/(rc.bottom-rc.top);

		for(iy=rc.top; iy<rc.bottom; iy++)
		{
			for(ix=rc.left; ix<rc.right; ix++)
			{
				float n = pn.Get(ix*xStep, iy*yStep)+0.5f*amplitude;
				m_height_map.set_value(ix, iy, unsigned short(fabsf(n)));
			}
		}//end of for	

		//--
		post_heightmap_change(rc);
	}

	void nterrain_actor::import_heightmap(const nstring& img_file_name)
	{
		nimage img;
		img.load_from_file(img_file_name);
		img.convert_format(EPF_R8G8B8);

		size_t img_w = img.get_width();
		size_t img_h = img.get_height();

		if(img_w != m_height_map.get_width()
			||img_h != m_height_map.get_height())
		{
			nthrow(_T("Image-Terrain Size Mismatch."));
		}

		pixel_R8G8B8* pixel = static_cast<pixel_R8G8B8*>(img.get_pixel());

		for(size_t y=0; y<img_h; y++)
		{
			for(size_t x=0; x<img_w; x++)
			{
				m_height_map.set_value(x, y, pixel->R);
				pixel++;
			}
		}

		img.destroy();

		//--
		post_heightmap_change( nrect(0,0,img_w,img_h) );			
	}

	

	bool nterrain_actor::line_check(ncheck_result& ret, const vector3& start, const vector3& end, ELineCheckType check_type)
	{
		if( !(check_type&ELCT_Terrain) )
			return false;

		if( !m_height_map.is_valid() )
			return false;

		bool find = false;
		npoint pt = world2tile(start.x, start.z);		
		int old_side = 0; 
		float step = std::min(m_space.scale.x, m_space.scale.z);		
		float test_len = step;
		float max_test_len = vec_distance(start, end);
		vector3 line_dir = vec_normalize(end-start);

		vector3 wpt;
		while(test_len < max_test_len)
		{
			wpt = start + line_dir*test_len;
			pt = world2tile(wpt.x, wpt.z);

			if( m_height_map.is_valid_index(pt.x, pt.y) )
			{			
				if(old_side == 0)
					old_side = classify_side(wpt); // first valid side
				else
				{
					if(classify_side(wpt) != old_side)
					{
						find = true;
						ret.location = tile2world(pt.x, pt.y);
						ret.dist = test_len;
						break;
					}
				}
			}

			test_len += step;
		}

		return find;
	}

	int nterrain_actor::classify_side(const vector3& wpt)
	{
		float map_h = get_height(wpt.x, wpt.z);

		if(map_h > wpt.y)
			return -1;
		else
			return 1;
	}

	float nterrain_actor::get_height(float wx, float wz) const
	{
		if( !m_height_map.is_valid() )
			return 0;

		const vector3& scale = m_space.scale;
		const vector3& pos = m_space.location;

		npoint pt = world2tile(wx, wz);
		float u = wx - (pt.x*scale.x+pos.x);
		float v = wz - (pt.y*scale.z+pos.z);

		u /= scale.x;
		v /= scale.z;

		/*	1---2
			|	|
			4---3		*/
		float p00 = m_height_map.get_value_safe(pt.x, pt.y);
		float p10 = m_height_map.get_value_safe(pt.x+1, pt.y);
		float p01 = m_height_map.get_value_safe(pt.x, pt.y+1);
		float p11 = m_height_map.get_value_safe(pt.x+1, pt.y+1);

		return quad_lerp(p00, p10, p01, p11, u, v)*scale.y+pos.y;
	}

	void nterrain_actor::post_heightmap_change(const nrect& region)
	{
		if( !m_height_map.is_valid() )
			return;

		if(region.get_width() > 0
			&& region.get_height() > 0)
		{
			// 目前是一个简单的遍历，如果需要的话可以使用quad tree去优化
			for( st_chunk_array::iterator iter = m_chunks.begin();
				iter != m_chunks.end();
				++iter )
			{
				(*iter)->post_heightmap_change(region);
			}// end of for()
		}
	}

	void nterrain_actor::post_vizmap_change(const nrect& region)
	{
		if(region.get_width() > 0
			&& region.get_height() > 0)
		{
			for( st_chunk_array::iterator iter = m_chunks.begin();
				iter != m_chunks.end();
				++iter )
			{
				(*iter)->post_vizmap_change(region);
			}// end of for()
		}
	}

	void nterrain_actor::_post_material_create()
	{
		for( st_chunk_array::iterator iter = m_chunks.begin();
			iter != m_chunks.end();
			++iter )
		{
			(*iter)->_post_material_create(&m_mtl_setup);
		}// end of for()
	}

	void nterrain_actor::_post_layer_alpha_change(const nstring& layer_name, const nrect& region)
	{
		if(region.get_width() > 0
			&& region.get_height() > 0)
		{
			for( st_chunk_array::iterator iter = m_chunks.begin();
				iter != m_chunks.end();
				++iter )
			{
				(*iter)->_post_layer_alpha_change(layer_name, region);
			}// end of for()
		}
	}

	//void nterrain_actor::post_deco_layer_change( nterrain_deco_layer* layer,const nrect& region )
	//{
	//	if(region.get_width() > 0
	//		&& region.get_height() > 0)
	//	{
	//		for( st_chunk_array::iterator iter = m_chunks.begin();
	//			iter != m_chunks.end();
	//			++iter )
	//		{
	//			(*iter)->_post_deco_layer_change(layer, region);
	//		}// end of for()
	//	}
	//}

	void nterrain_actor::_post_terrain_layer_change()
	{
		for( st_chunk_array::iterator iter = m_chunks.begin();
			iter != m_chunks.end();
			++iter )
		{
			(*iter)->_post_layer_change();
		}// end of for()
	}

	void nterrain_actor::serialize(narchive& ar)
	{
		nactor::serialize_basic_property(ar);

		//------------------------------------------
		nSERIALIZE(ar, m_chunk_size);

		//-- height map属性
		int hmap_w=0, hmap_h = 0;
		if( ar.is_loading() )
		{
			nSERIALIZE(ar, hmap_w);
			nSERIALIZE(ar, hmap_h);			
		}
		else
		{
			hmap_w = m_height_map.get_width();
			hmap_h = m_height_map.get_height();
			nSERIALIZE(ar, hmap_w);
			nSERIALIZE(ar, hmap_h);
		}
		

		//-- 存取height map
		nstring pkg_name = ar.get_file()->get_package();
		nstring file_name = ar.get_file()->get_file_name();
		nstring folder;
		nstring hmap_file_name;

		nstring::size_type fpos = file_name.find_last_of(_T("/"));
		if( fpos != nstring::npos )
			folder = file_name.substr(0, fpos+1);
		
		hmap_file_name = folder+m_name.name_str;
		hmap_file_name += _T("_hmap.raw");

		if( ar.is_loading() )
		{
			m_height_map.create(hmap_w, hmap_h, 0);
			load_height_map_as_raw(nengine::instance()->get_file_sys(),
				m_height_map, pkg_name, hmap_file_name);
		}
		else
		{
			save_height_map_as_raw(nengine::instance()->get_file_sys(),
				m_height_map, pkg_name, hmap_file_name);
		}		

		//-- 存取viz map
		nstring vmap_file_name = folder+m_name.name_str;
		vmap_file_name += _T("_vmap.raw");

		nfile_system* fs = nengine::instance()->get_file_sys();
		if( ar.is_loading() )
		{
			m_viz_map.create(hmap_w, hmap_h, true);
			m_viz_map.load_raw(fs, pkg_name, vmap_file_name);
		}
		else
		{
			m_viz_map.save_raw(fs, pkg_name, vmap_file_name);
		}

		//-- 存取材质
		if( ar.is_loading() )
		{
			m_mtl_setup.reset( this );
			m_deco_setup.reset(this);
		}
		nstring mtl_setup_class = m_mtl_setup.reflection_get_class()->get_name();
		ar.object_begin(_T("m_mtl_setup"), mtl_setup_class);
		m_mtl_setup.serialize(ar);
		ar.object_end();

		nstring deco_setup_class = m_deco_setup.reflection_get_class()->get_name();
		ar.object_begin(_T("m_deco_setup"), deco_setup_class);
		m_deco_setup.serialize(ar);
		ar.object_end();
	}

	void nterrain_actor::_level_loaded(nlevel* level_ptr)
	{
		m_owner = level_ptr;
		
		//--
		create_chunks();	
		_post_material_create();
		_post_terrain_layer_change();
	}

	void nterrain_actor::_destroy()
	{
		nactor::_destroy();

		m_height_map.destroy();				
		m_viz_map.destroy();
		m_mtl_setup.reset(this);
		m_chunks.clear();
	}


	// physics interfaces
	void nterrain_actor::init_phys()
	{
		if( m_phys_body_instance.get() != NULL )
		{
			nLog_Error( _T("Recreate physics actor...") );
			return;
		}

		NxPhysicsSDK* nx_sdk = nengine::instance()->get_phys_engine()->get_nx_physics_sdk();
		if( nx_sdk == NULL ) return;

		nlevel* level_owner = get_owner();
		if( level_owner == NULL )return;
		nphys_scene* phys_scene = level_owner->get_phys_scene();
		if( phys_scene == NULL || !phys_scene->is_valid_scene() ) return;
		NxScene* nx_scene = phys_scene->get_nx_scene();

		int nb_columns = m_height_map.get_width();
		int nb_rows = m_height_map.get_height();
		const vector3&  terrain_scale = get_scale();


		int default_material = phys_scene->get_default_mat()->getMaterialIndex(); 
		int hole_material = default_material + 1; 

		NxHeightFieldDesc HFDesc;
		HFDesc.nbColumns		= nb_columns;
		HFDesc.nbRows			= nb_rows;
		HFDesc.samples			= new NxHeightFieldSample[nb_columns*nb_rows];
		HFDesc.sampleStride		= sizeof(NxHeightFieldSample);
		HFDesc.flags			= NX_HF_NO_BOUNDARY_EDGES;

		char* curr_byte = (char*)HFDesc.samples;
		for ( int row_index=0; row_index<nb_rows; row_index++ )
		{
			for ( int col_index=0; col_index<nb_columns; col_index++ )
			{
				NxHeightFieldSample* curr_sample = (NxHeightFieldSample*)curr_byte;
				curr_sample->height = m_height_map.get_value( row_index, col_index);
				curr_sample->tessFlag = NX_HF_0TH_VERTEX_SHARED;

				//@todo: set hold Material
				curr_sample->materialIndex0 = default_material;
				curr_sample->materialIndex1 = default_material;

				curr_byte += HFDesc.sampleStride;
			}
		}

		m_phys_height_field = nx_sdk->createHeightField(HFDesc);

		// data has been copied, we can free our buffer
		delete [] HFDesc.samples;

		if ( m_phys_height_field == NULL )
		{
			nLog_Error(_T("Can not create height field ..."));
			return;
		}

		NxHeightFieldShapeDesc TerrainShapeDesc;
		TerrainShapeDesc.heightField	= m_phys_height_field;
		TerrainShapeDesc.shapeFlags		= NX_SF_FEATURE_INDICES | NX_SF_VISUALIZATION;
		TerrainShapeDesc.heightScale	= terrain_scale.y * physx::engine_to_nx_scale;
		TerrainShapeDesc.rowScale		= terrain_scale.x * physx::engine_to_nx_scale;
		TerrainShapeDesc.columnScale	= terrain_scale.z * physx::engine_to_nx_scale;
		TerrainShapeDesc.meshFlags		= 0;
		TerrainShapeDesc.materialIndexHighBits = 0;
		TerrainShapeDesc.holeMaterial	= hole_material;
		TerrainShapeDesc.groupsMask		= physx::create_groups_mask(PhysCC_Default, NULL);

		//@ todo: Per-triangle materials for terrain...

		// Create actor description and instance it.
		NxActorDesc TerrainActorDesc;
		TerrainActorDesc.shapes.pushBack(&TerrainShapeDesc);
		TerrainActorDesc.body		   = NULL;
		TerrainActorDesc.globalPose.t = physx::nx_vector_conv(m_space.location);

		TerrainActorDesc.compartment = phys_scene->get_rigidbody_compartment();
		nASSERT( TerrainActorDesc.isValid() );
		NxActor* terrain_actor = nx_scene->createActor(TerrainActorDesc);
		if(terrain_actor)
		{
			m_phys_body_instance.reset( nconstruct<nphys_body_instance>( _T("nphys_body_instance") ) );
			m_phys_body_instance->m_nx_actor = terrain_actor;
			m_phys_body_instance->m_owner_cmp = NULL;
			m_phys_body_instance->m_owner_actor = this;

			terrain_actor->userData = &m_phys_body_instance;
		}
		else
		{
			nLog_Error( _T("Could not create terrain NxActor..") );
		}
	}

	void nterrain_actor::simulate_phys(float /*delta_time*/)
	{

	}

	void nterrain_actor::release_phys()
	{
		nlevel* level_owner = get_owner();
		if( level_owner == NULL )return;
		NxPhysicsSDK* nx_sdk = nengine::instance()->get_phys_engine()->get_nx_physics_sdk();
		if( nx_sdk == NULL )return;
		nphys_scene* phys_scene = level_owner->get_phys_scene();
		if( phys_scene == NULL || !phys_scene->is_valid_scene() ) return;

		if( m_phys_body_instance.get() != NULL )
		{
			if ( m_phys_body_instance->m_nx_actor )
			{
				phys_scene->get_nx_scene()->releaseActor( *(m_phys_body_instance->m_nx_actor) );
			}
		}
		if( m_phys_height_field != NULL )
		{
			nx_sdk->releaseHeightField(*m_phys_height_field);
		}
		m_phys_body_instance.reset();
	}

	nactor::ptr nterrain_actor::clone()
	{
		return nactor::ptr();
	}

	nterrain_chunk::ptr nterrain_actor::get_chunk( float x,float y )
	{
		npoint pt = world2tile(x, y);
		size_t w = m_height_map.get_width();
		size_t h = m_height_map.get_height();

		int yc = (h-1)/(m_chunk_size-1);
		int xc = (w-1)/(m_chunk_size-1);

		int cx=pt.x/(m_chunk_size-1);	if(cx>=xc) return nterrain_chunk::ptr();
		int cy=pt.y/(m_chunk_size-1);	if(cy>=yc) return nterrain_chunk::ptr();

		return m_chunks[cy*yc+cx];		
	}

}//namespace nexus