#include "StdAfx.h"
#include "..\nengine\renderer\nrender_mesh.h"
#include "../nengine/framework/view_info.h"
#include "nrender_light.h"
#include "drawing_light.h"
#include "..\nengine\actor\nlight_component.h"
#include "..\nengine\actor\nprimitive_component.h"
#include "drawing_pre_pass.h"
#include "nrenderer_d3d9.h"
#include "d3d_device_manager.h"
#include "scene_render_targets.h"
#include "..\ncore\math\nfrustum.h"
#include "util.h"

namespace nexus
{
#define SAFE_RELEASE(p) if(p) { p->Release(); p = NULL; }
	class shadow_buffer_gen_filter : public drawing_filter
	{
	public:
		static bool should_draw(const nprimitive_component* obj)
		{
			return obj->cast_dynamic_shadow();
		}
	};

	class light_filter : public drawing_filter
	{
	public:
		static bool should_draw(const nprimitive_component* obj)
		{
			return obj->accept_dynamic_light();
		}
	};

	npoint_light_proxy::npoint_light_proxy(npoint_light_component* owner)
		:m_owner(owner)
	{
		m_drawing_policy[0] = new drawing_light<point_light_policy,0>(this);
		m_drawing_policy[1] = new drawing_light<point_light_policy,1>(this);
		m_drawing_policy[2] = new drawing_light<point_light_policy,2>(this);
	}

	npoint_light_proxy::~npoint_light_proxy()
	{
		for (int i = 0; i < 3; i ++)
		{
			delete m_drawing_policy[i];
		}
	}

	void npoint_light_proxy::render(nrenderer_d3d9* renderer)
	{
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();

		IDirect3DSurface9* old_surf = NULL;
		HRESULT hr = device->GetRenderTarget(0, &old_surf);
		IDirect3DSurface9* old_depth = NULL;
		hr = device->GetDepthStencilSurface(&old_depth);
		D3DVIEWPORT9 m_old_viewport;
		hr = device->GetViewport(&m_old_viewport);

		drawing_pre_pass dp;

		scene_render_targets* rt = scene_render_targets::instance();
	
		if (renderer->get_view_info()->enable_dynamic_shadow
			 && m_owner->project_shadow)
		{
			D3DVIEWPORT9 new_vp;
			new_vp.X = 0;
			new_vp.Y = 0;
			new_vp.Width  = (DWORD)1024;
			new_vp.Height = (DWORD)1024;
			new_vp.MinZ = 0.0f;
			new_vp.MaxZ = 1.0f;

			for( DWORD faceIndex = 0; faceIndex < 6; faceIndex++ )
			{
				render_package<shadow_buffer_gen_filter> shadow_package;
				for (prim_set::iterator iter = m_owner->affect_prims[faceIndex].begin();
					iter != m_owner->affect_prims[faceIndex].end();
					++iter)
				{
					(*iter)->render(&shadow_package);
				}
		
				IDirect3DSurface9* cube_face;
				rt->m_cube_texture->GetCubeMapSurface(( D3DCUBEMAP_FACES )faceIndex,0,&cube_face);
				device->SetRenderTarget(0,cube_face);
				device->SetDepthStencilSurface(scene_render_targets::instance()->m_cube_depth.get());
				cube_face->Release();	

				device->SetViewport(&new_vp);
				device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 0, 0, 0 ), 1.0f,0 );
				
				shadow_package.begin();
				shadow_package.draw_Opaque(&m_owner->view_infos[faceIndex],&dp);
			}
		}

		render_package<light_filter> light_package;
		for (prim_set::iterator iter = m_owner->affect_prim_set.begin();
			iter != m_owner->affect_prim_set.end();
			++iter)
		{
			nprimitive_component* prim = *iter;
			if (renderer->m_view_info.frustum.intersect_bounds(prim->get_bounds()))
			{
				prim->render(&light_package);
			}
		}

		hr = device->SetRenderTarget(0,old_surf);
		hr = device->SetDepthStencilSurface(old_depth);
		hr = device->SetViewport(&m_old_viewport);

		matrix44 world;
		mat_set_scaling(world,m_owner->m_attenuation.x,m_owner->m_attenuation.x,m_owner->m_attenuation.x);
		mat_translate(world,m_owner->m_position);

		draw_stencil_volume(
			g_sphere.positions,
			&g_sphere.indexes[0],
			g_sphere.indexes.size()/3,
			world,
			renderer->m_view_info.mat_view,
			renderer->m_view_info.mat_project
			);
		
		int shadow_policy = m_owner->project_shadow? 2:0;
		
		light_package.begin();
		light_package.draw_Opaque(&renderer->m_view_info,m_drawing_policy[shadow_policy]);
		device->SetRenderState(D3DRS_STENCILENABLE,false);
		device->Clear(0, NULL, D3DCLEAR_STENCIL, 0, 0, 0);
		
		old_surf->Release();
		old_depth->Release();
	}

	void npoint_light_proxy::draw_setup_effect(d3d9_shading_effect* effect_ptr)	const
	{
		vector4	color(m_owner->m_color.R,m_owner->m_color.G,m_owner->m_color.B,m_owner->m_color.A);
		color*=color.w;
		color.w=1;
		effect_ptr->set_vector("LGT_Color",color);
		effect_ptr->set_vector("LGT_Position", m_owner->m_position);		
		effect_ptr->set_vector("LGT_Attenuation",m_owner->m_attenuation);
		effect_ptr->set_d3d_texture("SM_CubeShadowBuffer",scene_render_targets::instance()->m_cube_texture.get());
	    effect_ptr->set_d3d_texture("SM_RandomRot",nrenderer_d3d9::instance()->get_texture(ETexture_Rot));
	}	

	ndirectional_light_proxy::ndirectional_light_proxy(ndirectional_light_component* owner)
		:m_owner(owner)
	{	
		m_drawing_policy[0] = new drawing_light<directional_light_policy,0>(this);
		m_drawing_policy[1] = new drawing_light<directional_light_policy,1>(this);
		m_drawing_policy[2] = new drawing_light<directional_light_policy,2>(this);
	}

	ndirectional_light_proxy::~ndirectional_light_proxy()
	{
		for (int i = 0; i < 3; i ++)
		{
			delete m_drawing_policy[i];
		}
	}

	void calculate_frustum_corners(vector<vector3>& corners,
		const vector3 &vSource,
		const vector3 &vTarget,
		const vector3 &vUp,
		float fNear, float fFar,
		float fFOV,float fAspect
		)
	{
		vector3 vZ=vTarget-vSource;
		vZ = vec_normalize(vZ);

		vector3 vX = vec_normalize(vec_cross(vUp,vZ));
		vector3 vY = vec_normalize(vec_cross(vZ,vX));

		float fNearPlaneHeight = tanf((fFOV) * 0.5f) * fNear;
		float fNearPlaneWidth = fNearPlaneHeight * fAspect;

		float fFarPlaneHeight = tanf((fFOV) * 0.5f) * fFar;
		float fFarPlaneWidth = fFarPlaneHeight * fAspect;

		vector3 vNearPlaneCenter = vSource + vZ * fNear;
		vector3 vFarPlaneCenter = vSource + vZ * fFar;

		corners[0]=vector3(vNearPlaneCenter - vX*fNearPlaneWidth + vY*fNearPlaneHeight);
		corners[1]=vector3(vNearPlaneCenter - vX*fNearPlaneWidth - vY*fNearPlaneHeight);
		corners[2]=vector3(vNearPlaneCenter + vX*fNearPlaneWidth - vY*fNearPlaneHeight);
		corners[3]=vector3(vNearPlaneCenter + vX*fNearPlaneWidth + vY*fNearPlaneHeight);

		corners[4]=vector3(vFarPlaneCenter - vX*fFarPlaneWidth + vY*fFarPlaneHeight);
		corners[5]=vector3(vFarPlaneCenter - vX*fFarPlaneWidth - vY*fFarPlaneHeight);
		corners[6]=vector3(vFarPlaneCenter + vX*fFarPlaneWidth - vY*fFarPlaneHeight);
		corners[7]=vector3(vFarPlaneCenter + vX*fFarPlaneWidth + vY*fFarPlaneHeight);
	}

	void ndirectional_light_proxy::calculate_split_distances(float shadow_near,float shadow_far,int split_num)
	{
		float lambda = 0.5;

		split_pos.resize(split_num + 1);
		for(int i=0;i< split_num;i++)
		{
			float fIDM=i/(float)split_num;
			float fLog=shadow_near*powf((shadow_far/shadow_near),fIDM);
			float fUniform=shadow_near+(shadow_far-shadow_near)*fIDM;
			split_pos[i]=fLog*lambda+fUniform*(1-lambda);
		}

		// make sure border values are right
		split_pos[0]=shadow_near + 0.001f;
		split_pos[split_num]=shadow_far - (shadow_far - shadow_near)*0.01f;
	}

	void ndirectional_light_proxy::calculate_light_for_frustum(const vector<vector3>& corners,const nview_info& camera_view)
	{
		float fMaxX=-FLT_MAX;
		float fMaxY=-FLT_MAX;
		float fMinX=FLT_MAX;
		float fMinY=FLT_MAX;
		float fMaxZ=0;
		float fMinZ=FLT_MAX;

		float light_near = 10.0f;
		float light_far = HALF_WORLD_MAX;

		matrix44 inv_view = m_owner->local_to_world;

		vector3 view_pos =  - vec_normalize(inv_view.get_axis_z())*HALF_WORLD_MAX; 
		inv_view.set_origin(view_pos);

		m_lgt_view.mat_view = mat_inverse(inv_view);
		nexus::mat_set_perspective_LH(m_lgt_view.mat_project,nPI/2,1,light_near,light_far);

		// for each corner point
		for(size_t i=0;i<corners.size();i++)
		{
			// transform point
			vector4 vTransformed(corners[i]);
			vTransformed = vTransformed*m_lgt_view.mat_view*m_lgt_view.mat_project;

			// project x and y
			vTransformed.x/=vTransformed.w;
			vTransformed.y/=vTransformed.w;

			// find min and max values
			if(vTransformed.x>fMaxX) fMaxX=vTransformed.x;
			if(vTransformed.y>fMaxY) fMaxY=vTransformed.y;
			if(vTransformed.y<fMinY) fMinY=vTransformed.y;
			if(vTransformed.x<fMinX) fMinX=vTransformed.x;
			if(vTransformed.z<fMinZ) fMinZ=vTransformed.w;
			if(vTransformed.z>fMaxZ) fMaxZ=vTransformed.w;
		}
		
		light_near = fMinZ*0.99f;
		light_far = fMaxZ*1.01f;

		m_lgt_view.eye_pos = m_owner->local_to_world.get_origin();
		nexus::mat_set_perspective_LH(m_lgt_view.mat_project,nPI/2,1,light_near,light_far);
	
		float fScaleX=2.0f/(fMaxX-fMinX);
		float fScaleY=2.0f/(fMaxY-fMinY);

		float fOffsetX=-0.5f*(fMaxX+fMinX)*fScaleX;
		float fOffsetY=-0.5f*(fMaxY+fMinY)*fScaleY;

		matrix44 mCropView(		fScaleX,     0.0f,		0.0f,   0.0f,
								0.0f,		fScaleY,	0.0f,   0.0f,
								0.0f,		0.0f,		1.0f,   0.0f,
								fOffsetX,	fOffsetY,	0.0f,   1.0f );

		m_lgt_view.mat_project = m_lgt_view.mat_project*mCropView;

		m_lgt_view.mat_view_project = m_lgt_view.mat_view*m_lgt_view.mat_project;
	}

	static const UINT index[36] = {
		0,1,3,
		1,2,3,
		3,2,7,
		2,6,7,
		0,7,4,
		0,3,7,
		0,4,5,
		0,5,1,
		4,7,5,
		5,7,6,
		1,5,2,
		5,6,2
	};

	void ndirectional_light_proxy::render(nrenderer_d3d9* renderer)
	{
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		D3DVIEWPORT9 m_old_viewport;
		device->GetViewport(&m_old_viewport);
		if (renderer->get_view_info()->enable_dynamic_shadow
			 && m_owner->project_shadow)
		{
			drawing_pre_pass dp;
			
			//这里对视截体进行压缩，可以减小投影的范围
			const vector<nprimitive_component*>& light_prims = renderer->m_render_package.get_primitives();
			float max_dist = 0;
			float min_dist = renderer->m_view_info.zfar;
			vector3 pt_array[8];
			for (size_t i = 0; i < light_prims.size(); i ++)
			{
				nprimitive_component* prim = light_prims[i];
				if (prim->accept_dynamic_light())
				{
					const box_sphere_bounds& bound = prim->get_bounds();
					bound.get_corners(pt_array);
					for (int i = 0; i < 8; i ++)
					{
						float dist = (pt_array[i]*renderer->m_view_info.mat_view).z;
					    max_dist = max(dist,max_dist);
						min_dist =  min(dist,min_dist);
					}
				}
			}
			
			max_dist = min(renderer->m_view_info.zfar,max_dist);
			min_dist = max(renderer->m_view_info.znear,min_dist);

			if (max_dist - min_dist <= 0)
			{
				return;
			}
			
			//通过视截体的长度来决定切分的块数
			int split_num = 1;//min(floor((max_dist- min_dist)/8000.0f)+1,2);
			calculate_split_distances(min_dist,max_dist,split_num);

			// Render each split from front to back
			for(int iSplit=0;iSplit<split_num;iSplit++)
			{
				// near and far planes for current frustum split
						float fNear=split_pos[iSplit];
				float fFar=split_pos[iSplit+1];
				
				int shadow_policy = iSplit > 0 ? 0:1; 
				vector<vector3> corners(8);
				calculate_frustum_corners(corners,
					renderer->m_view_info.eye_pos,
					renderer->m_view_info.look_at,
					vector3(0,1,0),
					fNear,fFar,
					renderer->m_view_info.fov,
					renderer->m_view_info.aspect);

				if (shadow_policy > 0)
				{
					calculate_light_for_frustum(corners,renderer->m_view_info);

					//	 Enable rendering to shadowmap
					//nfrustum shadow_frustum;
					//shadow_frustum.extract(m_lgt_view.mat_view,m_lgt_view.mat_project);
					//render_package<shadow_buffer_gen_filter> shadow_package;
					////renderer->m_scene_ptr->get_visible_primitive(shadow_frustum,&shadow_package);
					//const vector<nprimitive_component*>& light_prims = renderer->m_render_package.get_primitives();
					//for (size_t i = 0; i < light_prims.size(); i ++)
					//{
					//	nprimitive_component* prim = light_prims[i];
					//	if (shadow_frustum.intersect_bounds(prim->get_bounds()))
					//	{
					//		prim->render(&shadow_package);
					//	}
					//}

					const rt_item&  rt_shadow= scene_render_targets::instance()->get_render_target(ERT_ShadowColor);
					vector2 size = rt_shadow.size();

					IDirect3DSurface9* old_surf = NULL;
					HRESULT hr = device->GetRenderTarget(0, &old_surf);
					IDirect3DSurface9* old_depth = NULL;
					hr = device->GetDepthStencilSurface(&old_depth);

					IDirect3DSurface9* shadow_surface = rt_shadow.surf.get();
					IDirect3DSurface9* shadow_depth = scene_render_targets::instance()->get_render_target(ERT_ShadowDepth).surf.get();
					hr = device->SetRenderTarget(0, shadow_surface);
					hr = device->SetDepthStencilSurface(shadow_depth);

					D3DVIEWPORT9 new_vp;
					new_vp.X = 0;
					new_vp.Y = 0;
					new_vp.Width  = (DWORD)size.x;
					new_vp.Height = (DWORD)size.y;
					new_vp.MinZ = 0.0f;
					new_vp.MaxZ = 1.0f;

					device->SetViewport(&new_vp);
					hr = device->Clear(0, NULL, D3DCLEAR_ZBUFFER|D3DCLEAR_TARGET, 0, 1, 0);

					device->SetRenderState(D3DRS_COLORWRITEENABLE, 0);

					//shadow_package.begin();
				    renderer->m_render_package.draw_Opaque(&m_lgt_view,&dp);

					hr = device->SetRenderTarget(0,old_surf);	
					hr = device->SetDepthStencilSurface(old_depth);
					hr = device->SetViewport(&m_old_viewport);
					old_surf->Release();
					old_depth->Release();
				}

					//使用蒙板优化填充率
			//	draw_stencil_volume(corners,index,12,matrix44::identity,renderer->m_view_info.mat_view,
			//		renderer->m_view_info.mat_project);

				//prim cull				
			//	nfrustum cull_frustum;
			//	matrix44 cur_project;
			//	mat_set_perspective_LH(cur_project,renderer->m_view_info.fov,renderer->m_view_info.aspect,fNear,fFar);
			//	cull_frustum.extract(renderer->m_view_info.mat_view,cur_project);

			//	render_package<light_filter> light_package;
			////	renderer->m_scene_ptr->get_visible_primitive(cull_frustum,&light_package);
			//	const vector<nprimitive_component*>& light_prims = renderer->m_render_package.get_primitives();
			//	for (size_t i = 0; i < light_prims.size(); i ++)
			//	{
			//		nprimitive_component* prim = light_prims[i];
			//		if (cull_frustum.intersect_bounds(prim->get_bounds()))
			//		{
			//			prim->render(&light_package);
			//		}
			//	}

				device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORALL);
				renderer->m_render_package.draw_Opaque(renderer->get_view_info(),m_drawing_policy[shadow_policy]);

				device->SetRenderState(D3DRS_STENCILENABLE,false);
				device->Clear(0, NULL, D3DCLEAR_STENCIL, 0, 0, 0);
			}
		}
		else
		{
			renderer->m_render_package.draw_Opaque(renderer->get_view_info(),m_drawing_policy[0]);
		}
	}

	void ndirectional_light_proxy::draw_setup_effect(d3d9_shading_effect* effect_ptr) const
	{
		vector4	color(m_owner->m_color.R,m_owner->m_color.G,m_owner->m_color.B,m_owner->m_color.A);
		color*=color.w;
		color.w=1;
		effect_ptr->set_vector("LGT_Color",color);
		effect_ptr->set_vector("LGT_DIRECTION", -m_owner->local_to_world.get_axis_z());	
		
		const rt_item&  rt_shadow= scene_render_targets::instance()->get_render_target(ERT_ShadowDepth);
		vector2 size = rt_shadow.size();

		float fOffsetX = 0.5f + (0.5f / (float)size.x);
		float fOffsetY = 0.5f + (0.5f / (float)size.y);
		unsigned int range = 1;            //note different scale in DX9!
		static float bias = 0.001f;
		matrix44 texScaleBiasMat(	0.5f,     0.0f,     0.0f,				0.0f,
															0.0f,    -0.5f,     0.0f,				0.0f,
															0.0f,     0.0f,     (float)range,	0.0f,
															fOffsetX, fOffsetY, -bias,    1.0f );

		effect_ptr->set_matrix("SM_Matrix", m_lgt_view.mat_view_project*texScaleBiasMat);
		effect_ptr->set_d3d_texture("SM_Buffer",rt_shadow.tex.get());
		
		float softness = m_owner->m_shadow_softness / size.x;
		effect_ptr->set_vector("SM_Jitter_Scale", vector4( size.x / (float)JITTER_SIZE, size.y / (float)JITTER_SIZE, 0,softness));
		effect_ptr->set_d3d_texture("SM_Jitter", nrenderer_d3d9::instance()->get_texture(ETexture_Jitter));
	}

	nspot_light_proxy::nspot_light_proxy(nspot_light_component* owner)
		:m_owner(owner)
	{
		m_drawing_policy[0] = new drawing_light<spot_light_policy,0>(this);
		m_drawing_policy[1] = new drawing_light<spot_light_policy,1>(this);
		m_drawing_policy[2] = new drawing_light<spot_light_policy,2>(this);
	}

	nspot_light_proxy::~nspot_light_proxy()
	{
		for (int i = 0; i < 3; i ++)
		{
			delete m_drawing_policy[i];
		}
	}

	void nspot_light_proxy::render(nrenderer_d3d9* renderer)
	{
		shadow_buffer_gen_filter filter;
		drawing_pre_pass dp;

		//prim cull
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		IDirect3DSurface9* old_surf = NULL;
		HRESULT hr = device->GetRenderTarget(0, &old_surf);
		IDirect3DSurface9* old_depth = NULL;
		hr = device->GetDepthStencilSurface(&old_depth);
		D3DVIEWPORT9 m_old_viewport;
		hr = device->GetViewport(&m_old_viewport);

		render_package<shadow_buffer_gen_filter> shadow_package;
		render_package<light_filter> light_package;	
		for (prim_set::iterator iter = m_owner->affect_prim_set.begin();
			iter != m_owner->affect_prim_set.end();
			++iter)
		{
			nprimitive_component* prim = *iter;
			prim->render(&shadow_package);
			if (renderer->m_view_info.frustum.intersect_bounds(prim->get_bounds()))
			{
				prim->render(&light_package);
			}
		}

		if (renderer->get_view_info()->enable_dynamic_shadow
			 && m_owner->project_shadow)
		{
			const rt_item&  rt_shadow= scene_render_targets::instance()->get_render_target(ERT_ShadowColor);
			vector2 size = rt_shadow.size();
			hr = device->SetRenderTarget(0, rt_shadow.surf.get());
			hr = device->SetDepthStencilSurface(scene_render_targets::instance()->get_render_target(ERT_ShadowDepth).surf.get());

			D3DVIEWPORT9 new_vp;
			new_vp.X = 0;
			new_vp.Y = 0;
			new_vp.Width  = (DWORD)size.x;
			new_vp.Height = (DWORD)size.y;
			new_vp.MinZ = 0.0f;
			new_vp.MaxZ = 1.0f;

			device->SetViewport(&new_vp);

			hr = device->Clear(0, NULL, D3DCLEAR_ZBUFFER|D3DCLEAR_TARGET, 0, 1, 0);
			device->SetRenderState(D3DRS_COLORWRITEENABLE, 0);
			shadow_package.begin();
			shadow_package.draw_Opaque(&m_owner->view_info,&dp);
		}

		hr = device->SetRenderTarget(0,old_surf);
		hr = device->SetDepthStencilSurface(old_depth);
		hr = device->SetViewport(&m_old_viewport);
		device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORALL);
		int shadow_policy = 1;//m_owner->project_shadow?2:0;
		light_package.begin();
		light_package.draw_Opaque(renderer->get_view_info(),m_drawing_policy[shadow_policy]);
		old_surf->Release();
		old_depth->Release();
	}

	void nspot_light_proxy::draw_setup_effect(d3d9_shading_effect* effect_ptr)	const
	{
		vector4	color(m_owner->m_color.R,m_owner->m_color.G,m_owner->m_color.B,m_owner->m_color.A);
		color*=color.w;
		color.w=1;
		effect_ptr->set_vector("LGT_Color",color);
		effect_ptr->set_vector("LGT_Position", m_owner->local_to_world.get_origin());		
		effect_ptr->set_vector("LGT_Attenuation",m_owner->m_attenuation);

		effect_ptr->set_vector("LGT_DIRECTION", vec_normalize(m_owner->local_to_world.get_axis_z()));	
		vector2 SpotAngles = vector2(m_owner->inner_cone_angle,m_owner->outer_cone_angle);
		SpotAngles*=3.14159265f/360.0f;
		SpotAngles = vector2(cos(SpotAngles.x),cos(SpotAngles.y));
		effect_ptr->set_vector("LGT_SPOT_ANGLES",SpotAngles);
		
		const rt_item&  rt_shadow= scene_render_targets::instance()->get_render_target(ERT_ShadowDepth);
		vector2 size = rt_shadow.size();

		float fOffsetX = 0.5f + (0.5f / (float)size.x);
		float fOffsetY = 0.5f + (0.5f / (float)size.y);
		unsigned int range = 1;            //note different scale in DX9!
		float fBias    = -0.00005f * (float)range;
		//float fBias    = 0.0f;
		matrix44 texScaleBiasMat(	0.5f,     0.0f,     0.0f,         0.0f,
									0.0f,    -0.5f,     0.0f,         0.0f,
									0.0f,     0.0f,     (float)range, 0.0f,
									fOffsetX, fOffsetY, fBias,        1.0f );

		effect_ptr->set_matrix("SM_Matrix", m_owner->view_info.mat_view_project*texScaleBiasMat);
		effect_ptr->set_d3d_texture("SM_Buffer",rt_shadow.tex.get());
	
		float softness = m_owner->m_shadow_softness / size.x;
		effect_ptr->set_vector("SM_Jitter_Scale", vector4(size.x / JITTER_SIZE, size.y / JITTER_SIZE, 0, softness));

		effect_ptr->set_d3d_texture("SM_Jitter", nrenderer_d3d9::instance()->get_texture(ETexture_Jitter));
	}
}