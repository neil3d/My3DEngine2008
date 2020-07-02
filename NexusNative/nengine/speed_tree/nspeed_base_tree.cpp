#include "StdAfx.h"
#include "nspeed_base_tree.h"
#include "../framework/nengine.h"
#include "../renderer/nrender_resource_manager.h"

#include "spt_cfg.h"

#ifdef _DEBUG
#	pragma comment(lib,"SPT_SDK/lib/vc9/debug/SpeedTreeRT_d.lib")
#else
#	pragma comment(lib,"SPT_SDK/lib/vc9/release/SpeedTreeRT.lib")
#endif

namespace nexus
{
	nDEFINE_NAMED_CLASS(nspeed_base_tree, nresource)

	nspeed_base_tree::nspeed_base_tree(const nstring& name_str):nresource(name_str),
		m_branches_mtl(_T("spt_branch_mtl")),
		m_frond_mtl(_T("spt_frond_mtl")),
		m_leaf_mtl(_T("spt_leaf_mtl"))
	{
		m_bounding_box.box_extent = vector3(WORLD_MAX,WORLD_MAX,WORLD_MAX);
		m_bounding_box.origin = vector3(0,0,0);
		m_bounding_box.sphere_radius = WORLD_MAX;

		m_has_branch = false;
		m_has_frond = false;
		m_has_leafcard = false;
		m_has_leafmesh = false;
	}

	nspeed_base_tree::~nspeed_base_tree(void)
	{
	}

	void nspeed_base_tree::load_from_file(const resource_location& loc)
	{
		// set texture orientation for DirectX
		CSpeedTreeRT::SetTextureFlip(true);

		// load SpeedWind first since we need a wind matrix count before we can compute the tree geometry
		// todo : (void) InitSpeedWind( );

		// set number of wind matrices to match SpeedWind
		CSpeedTreeRT::SetNumWindMatrices(c_nNumWindMatricesPerSource);
		

		//-- read file to memory lump
		nfile_system* fs = nengine::instance()->get_file_sys();
		int file_size = fs->get_file_size(loc.pkg_name, loc.file_name);
		if( file_size <= 0 )
			nthrow2(_T("Load SpeedTree failed!"), loc.to_string().c_str());
		
		nfile::ptr file_ptr = fs->open_file(loc.pkg_name, loc.file_name, EFileRead);

		boost::scoped_array<unsigned char> lump( new unsigned char[file_size] );
		file_ptr->read_buffer(lump.get(), file_size);

		//-- load tree ---------------------------------------------------------
		boost::shared_ptr<CSpeedTreeRT> spt( new CSpeedTreeRT );		

		bool ok = spt->LoadTree(lump.get(), file_size);
		if( !ok )
			nthrow2(_T("Load SpeedTree failed!"), loc.to_string().c_str());

		// make sure SpeedTreeRT generates normals
		spt->SetBranchLightingMethod(CSpeedTreeRT::LIGHT_DYNAMIC);
		spt->SetLeafLightingMethod(CSpeedTreeRT::LIGHT_DYNAMIC);
		spt->SetFrondLightingMethod(CSpeedTreeRT::LIGHT_DYNAMIC);

		// set the wind method to use the GPU
		spt->SetBranchWindMethod(CSpeedTreeRT::WIND_GPU);
		spt->SetLeafWindMethod(CSpeedTreeRT::WIND_GPU);
		spt->SetFrondWindMethod(CSpeedTreeRT::WIND_GPU);

		// override the size, if necessary
		// todo : spt->SetTreeSize(fSize, fSizeVariance);
		
		ok = spt->Compute();
		if( !ok )
			nthrow2(_T("SpeedTree Compute failed!"), loc.to_string().c_str());

		// make the leaves rock in the wind
		spt->SetLeafRockingState(true);

		// billboard setup
		spt->SetDropToBillboard(true);

		//-- create mesh ----------------------------------------------------
		create_branches(spt);
		create_frond(spt);
		create_leafcard(spt);
		create_leafmesh(spt);
		

		//-- create material ------------------------------------------------
		m_branches_mtl.create(spt, loc);
		m_frond_mtl.create(spt, loc);
		m_leaf_mtl.create(m_frond_mtl.get_texture(CSpeedTreeRT::TL_DIFFUSE),
			m_frond_mtl.get_texture(CSpeedTreeRT::TL_NORMAL));

		//-- bounds ---------------------------------------------------------
		compute_bounds(spt);

		//-- wind -----------------------------------------------------------
		// find general wind file; first look for a general wind specific to the SPT file that will be formatted
		// like "RedCedarRT_GeneralWind.ini"; if not found fallback to "GeneralWind.ini" and look in multiple places		
		file_size = fs->get_file_size( _T("engine_data"), _T("common/GeneralWind.ini") );
		if(file_size > 0
			&& file_size !=-1)
		{
			std::string txt;
			txt.resize(file_size+1, 0);
			nfile::ptr generalwind_file_ptr = fs->open_file(_T("engine_data"), _T("common/GeneralWind.ini"), EFileRead);
			generalwind_file_ptr->read_buffer((void*)txt.data(), file_size);
			
			bool wind_ok = m_cGeneralWind.Load(txt.c_str(), file_size);			
		}
		
		//--
		m_cGeneralWind.SetQuantities(c_nNumWindMatricesPerSource, c_nNumLeafAnglesPerSource);

		m_vWindMatrices.resize(c_nNumWindMatricesPerSource);
		m_vWindRustleAngles.resize(c_nNumLeafAnglesPerSource);
		m_vWindRockAngles.resize(c_nNumLeafAnglesPerSource);

		m_cGeneralWind.SetWindStrengthAndDirection(0.65f, 0.0f, 0.0f, 1.0f);

		for (int i = 0; i < int(m_vWindMatrices.size( )); ++i)
			m_vWindMatrices[i] = m_cGeneralWind.GetWindMatrix((unsigned int) i);
		

		//--
		m_location = loc;
		m_spt = spt;

	}

	void nspeed_base_tree::save_to_file(const resource_location& loc)
	{
		(void)loc;
		nASSERT(0 && "Not support");
	}

	void nspeed_base_tree::create_branches(boost::shared_ptr<CSpeedTreeRT> spt)
	{
		// query vertex attribute data
		CSpeedTreeRT::SGeometry sGeometry;
		spt->GetGeometry(sGeometry, SpeedTree_BranchGeometry);

		nspt_branch_vertex_data vert_data;
		if( vert_data.create(sGeometry) <= 0 )
			return;

		nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();

		render_res_ptr<nrender_static_mesh_indexed> new_render_mesh(
			rres_mgr->alloc_static_mesh_indexed() );

		// setup indices
		int nNumLods = spt->GetNumBranchLodLevels();
		int m = 0;
		for(int i=0; i<nNumLods; i++)
		{
			index_buffer16 indices;
			int num = vert_data.get_triangle_strip(sGeometry, i, indices);

			if( num > 2 )
			{
				if( m == 0)
				{
					new_render_mesh->create(EDraw_TriangleStrip, &vert_data, &indices, 0);
				}
				else
				{
					// lod triangles save as sections
					new_render_mesh->append_section(EDraw_TriangleStrip, &indices, 0);
				}
				m++;
			}
		}

		if( m > 0 )
		{
			m_has_branch = true;
			m_branches_mesh.reset( new_render_mesh.release() );
		}
	}

	void nspeed_base_tree::compute_bounds(boost::shared_ptr<CSpeedTreeRT> spt)
	{
		// query dimensions
		float afBoundingBox[6];
		spt->GetBoundingBox(afBoundingBox);

		vector3 box_min(afBoundingBox[0],afBoundingBox[1],afBoundingBox[2]);
		vector3 box_max(afBoundingBox[3],afBoundingBox[4],afBoundingBox[5]);
		m_bounding_box.origin = (box_max+box_min)*0.5f;
		m_bounding_box.box_extent = (box_max-box_min)*0.5f;
		m_bounding_box.sphere_radius = vec_length(m_bounding_box.box_extent);
	}

	void nspeed_base_tree::advance_speed_wind(float fTime)
	{
		m_cGeneralWind.Advance(fTime, true, false);

		// query matrices to be uploaded during the render
		nASSERT(int(m_vWindMatrices.size( )) == c_nNumWindMatricesPerSource);
		for (int i = 0; i < c_nNumWindMatricesPerSource; ++i)
			m_vWindMatrices[i] = m_cGeneralWind.GetWindMatrix((unsigned int) i);

		// query the rock/rustle angles
		nASSERT(int(m_vWindRockAngles.size( )) == c_nNumLeafAnglesPerSource);
		nASSERT(int(m_vWindRustleAngles.size( )) == c_nNumLeafAnglesPerSource);
		if (!m_vWindRockAngles.empty( ))
			(void) m_cGeneralWind.GetRockAngles(&m_vWindRockAngles[0]);
		if (!m_vWindRustleAngles.empty( ))
			(void) m_cGeneralWind.GetRustleAngles(&m_vWindRustleAngles[0]);

		//-- copy
		for (int i = 0; i < c_nNumWindMatricesPerSource; ++i)
		{
			nASSERT(m_vWindMatrices[i]);
			memcpy(&m_wnd_mat[i], m_vWindMatrices[i], 16 * sizeof(float));
		}


		//---------------
		nASSERT(int(m_vWindRockAngles.size( )) == c_nNumLeafAnglesPerSource);
		nASSERT(int(m_vWindRustleAngles.size( )) == c_nNumLeafAnglesPerSource);

		// upload leaf angle table
		if (m_vLeafAngleUploadTable.empty( ))
			m_vLeafAngleUploadTable.resize(4 * c_nNumLeafAnglesPerSource);
		float* pTablePtr = &m_vLeafAngleUploadTable[0];
		for (int i = 0; i < c_nNumLeafAnglesPerSource; ++i)
		{
			*pTablePtr++ = nDegToRad(m_vWindRockAngles[i]);
			*pTablePtr++ = nDegToRad(m_vWindRustleAngles[i]);
			pTablePtr += 2;
		}		
	}

	void nspeed_base_tree::create_frond(boost::shared_ptr<CSpeedTreeRT> spt)
	{
		int nNumLods = spt->GetNumFrondLodLevels( );
		if (nNumLods <= 0)
			return;

		// query vertex attribute data
		CSpeedTreeRT::SGeometry sGeometry;
		spt->GetGeometry(sGeometry, SpeedTree_FrondGeometry);

		nspt_frond_vertex_data vert_data;
		if( vert_data.create(sGeometry) <= 0 )
			return;
		

		nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();

		render_res_ptr<nrender_static_mesh_indexed> new_render_mesh(
			rres_mgr->alloc_static_mesh_indexed() );

		int m = 0;
		for(int i=0; i<nNumLods; i++)
		{
			index_buffer16 indices;
			if( vert_data.get_triangle_strip(sGeometry, i, indices) > 2)
			{
				if( m == 0)
				{
					new_render_mesh->create(EDraw_TriangleStrip, &vert_data, & indices, 0);
				}
				else
				{
					// lod triangles save as sections
					new_render_mesh->append_section(EDraw_TriangleStrip, &indices, 0);
				}
				m++;
			}
		}// end of for

		//--
		if( m > 0)
		{
			m_frond_mesh.reset( new_render_mesh.release() );
			m_has_frond = true;
		}
	}

	void nspeed_base_tree::create_leafcard(boost::shared_ptr<CSpeedTreeRT> spt)
	{
		nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();

		const int nNumLeafLods = spt->GetNumLeafLodLevels( );

		CSpeedTreeRT::SGeometry sGeometry;
		spt->GetGeometry(sGeometry, SpeedTree_LeafGeometry);

		for (int nLod = 0; nLod < nNumLeafLods; ++nLod)
		{
			const CSpeedTreeRT::SGeometry::SLeaf& sLeaves = sGeometry.m_pLeaves[nLod];

			// grab these parameters for shader uploading later
			if (nLod == 0)
			{
				float	m_fLeafRockScalar;          // one rock/rustle table is uploaded for all trees, these values help
				float   m_fLeafRustleScalar;        // scale the table up or down to how the tree was tuned in CAD

				m_fLeafRockScalar = sLeaves.m_fLeafRockScalar;
				m_fLeafRustleScalar = sLeaves.m_fLeafRustleScalar;
				m_leafcard_angle_scalars.x = m_fLeafRockScalar;
				m_leafcard_angle_scalars.y = m_fLeafRustleScalar;
			}

			if( sLeaves.m_nNumLeaves > 0 )
			{
				nspt_leafcard_vertex_data vert_data;
				if( vert_data.create(sLeaves) > 0 )
				{
					render_res_ptr<nrender_static_mesh_indexed> lod_mesh(
						rres_mgr->alloc_static_mesh_indexed() );
			
					index_buffer16 index_data;
					size_t num_index = vert_data.get_num_vert();
					for (size_t i = 0; i < num_index; i ++)
					{
						index_data.append_index(num_index - i - 1);
					}

					lod_mesh->create(EDraw_TriangleList, &vert_data,&index_data,0);
					m_leafcard_lods.push_back( lod_mesh.release() );
					m_has_leafcard = true;
				}
			}
		}// end of for
	}

	void nspeed_base_tree::create_leafmesh(boost::shared_ptr<CSpeedTreeRT> spt)
	{
		CSpeedTreeRT::SGeometry sGeometry;
		spt->GetGeometry(sGeometry, SpeedTree_LeafGeometry);

		nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();
		render_res_ptr<nrender_static_mesh_indexed> new_render_mesh(
			rres_mgr->alloc_static_mesh_indexed() );

		int mesh = 0;
		if (sGeometry.m_nNumLeafLods > 0)
		{			
			for (int nLod = 0; nLod < sGeometry.m_nNumLeafLods; ++nLod)
			{
				const CSpeedTreeRT::SGeometry::SLeaf* pLod = sGeometry.m_pLeaves + nLod;

				index_buffer16 indices;
				nspt_leafmesh_vertex_data vert_data;
				
				int v = vert_data.create(*pLod, indices);
				if( v>0 && indices.get_index_count() > 2)
				{
					if( mesh == 0)
					{
						new_render_mesh->create(EDraw_TriangleList, &vert_data, & indices, 0);
					}
					else
					{
						// lod triangles save as sections
						new_render_mesh->append_section(EDraw_TriangleList, &indices, 0);
					}
					mesh++;
				}// end of if
			}// end of for
		}// end of if

		//--
		if( mesh > 0 )
		{
			m_leafmesh.reset( new_render_mesh.release() );
			m_has_leafmesh = true;
		}
	}

	void nspeed_base_tree::_on_device_lost(int param)
	{
		m_branches_mesh.reset();
		m_frond_mesh.reset();
		m_leafcard_lods.clear();
		m_leafmesh.reset();
	}

	bool nspeed_base_tree::_on_device_reset(int param)
	{
		if( m_spt )
		{
			create_branches(m_spt);
			create_frond(m_spt);
			create_leafcard(m_spt);
			create_leafmesh(m_spt);
		}
		return true;
	}
}//namespace nexus