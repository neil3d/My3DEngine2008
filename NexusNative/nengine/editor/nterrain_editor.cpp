#include "StdAfx.h"
#include <ostream>
#include "nterrain_editor.h"
#include "neditor_cmd_mgr.h"
#include "../renderer/nrender_element.h"
#include "../util/perlin.h"
#include "terrain_brush.h"


namespace nexus
{
	const float TRN_BRUSH_CURSOR_ZBIAS = 0.04f;
	//-----------------------------------------------------------------------------------------------
	// 一个全局的noise对象，只在某些时刻重新生成；用于brush operator对象
	boost::shared_ptr<Perlin> g_brush_noise;
	static void _reset_brush_noise(float freq, int brush_strength)
	{
		g_brush_noise.reset( new Perlin(8, freq, brush_strength*0.5f, rand())
			);
	}

	//-----------------------------------------------------------------------------------------------
	// 笔刷编辑命令
	/** 编辑高度图的command基类 */
	template<typename T>
	class ntrn_brush_cmd : public neditor_cmd
	{
	public:
		ntrn_brush_cmd(void):m_dirty_region(0,0,0,0)
		{}
		virtual ~ntrn_brush_cmd(void)	{}

		bool init(nterrain_actor::ptr trn, nheight_map<T>* target, const nterrain_brush& brush, npoint cursor_pos)
		{
			if( !trn )
				return false;
			if( !target )
				return false;

			m_trn = trn;
			m_target = target;
			m_brush = brush;
			m_cursor_pos = cursor_pos;

			return true;
		}

		virtual bool execute(void)
		{
			nterrain_actor::ptr trn(m_trn);
			//-- backup old heightmap

			//-- change heightmap
			switch(m_brush.op)
			{
			case EOP_Paint:
				{
					trn_brush_add<T> brush_op;
					m_dirty_region = do_terrain_brush(m_target,
						m_cursor_pos,
						m_brush,
						brush_op);
				}
				break;
			case EOP_Flatten:
				{
					trn_brush_average<T> avg;
					
					do_terrain_brush(m_target,
						m_cursor_pos,
						m_brush,
						avg);
					if( avg.get_count() > 0 )
					{
						trn_brush_apply_val<T> apply(avg.get_result());

						m_dirty_region = do_terrain_brush(m_target,
							m_cursor_pos,
							m_brush,
							apply);						
					}
				}
				break;
			case EOP_Smooth:
				{
					trn_brush_smooth<T> brush_op(5);
					m_dirty_region = do_terrain_brush(m_target,
						m_cursor_pos,
						m_brush,
						brush_op);
					brush_op.apply(m_target);
				}
				break;
			case EOP_Apply:
				{
					trn_brush_apply_val<T> brush_op(m_brush.strength);
					m_dirty_region = do_terrain_brush(m_target,
						m_cursor_pos,
						m_brush,
						brush_op);					
				}
				break;
			case EOP_Noise:
				{
					nrect brush_rect;
					brush_rect.left = m_cursor_pos.x-m_brush.outer_radius;
					brush_rect.top = m_cursor_pos.y-m_brush.outer_radius;
					brush_rect.right = m_cursor_pos.x+m_brush.outer_radius;
					brush_rect.bottom = m_cursor_pos.y+m_brush.outer_radius;					
					_clip_rect(brush_rect, trn->get_map_width(), trn->get_map_height());

					trn_brush_noise<T> brush_op(brush_rect.get_width(), 
						brush_rect.get_height(),
						npoint(brush_rect.left, brush_rect.top)
						);
					m_dirty_region = do_terrain_brush(m_target,
						m_cursor_pos,
						m_brush,
						brush_op);					
				}
				break;
			}// end of switch

			return true;
		}

		virtual bool undo(void)
		{
			nterrain_actor::ptr trn( m_trn );

			// terrain对象已经被释放？
			if( !trn )
				return false;

			// write data

			return true;
		}

		const nrect& get_dirty_region() const	{	return m_dirty_region; }
	
	protected:
		boost::weak_ptr<nterrain_actor>	m_trn;
		nheight_map<T>*	m_target;
		nterrain_brush	m_brush;
		npoint			m_cursor_pos;

		nrect	m_dirty_region;
	};

	//-----------------------------------------------------------------------------------------------
	/** 编辑修改地形的高度图 */
	class ntrn_brush_cmd_hmap : public ntrn_brush_cmd<unsigned short>
	{
	public:
		ntrn_brush_cmd_hmap()	{}
		virtual ~ntrn_brush_cmd_hmap()	{}

		virtual bool execute(void)
		{
			nterrain_actor::ptr trn( m_trn );
			if( !trn )
				return false;

			if( ntrn_brush_cmd::execute() )
			{
				trn->post_heightmap_change(m_dirty_region);
				return true;
			}

			return false;
		}
	};

	//-----------------------------------------------------------------------------------------------
	/** 编辑修改texture splatting的图层alpha*/
	class ntrn_brush_cmd_alpha : public ntrn_brush_cmd<unsigned char>
	{
	public:
		ntrn_brush_cmd_alpha()	{}
		virtual ~ntrn_brush_cmd_alpha()	{}

		bool init(nterrain_actor::ptr trn, const nterrain_brush& brush, npoint cursor_pos)
		{
			//-- 记录材质指针
			nmaterial_base* mtl_base_ptr = trn->get_material();
			ntexture_splatting* splat = ndynamic_cast<ntexture_splatting>(mtl_base_ptr);
			if( splat == NULL )
				return false;

			m_mtl = splat;

			//-- 调用基类功能
			return ntrn_brush_cmd::init(trn, splat->get_alpha_map(brush.target_index), brush, cursor_pos);			
		}

		virtual bool execute(void)
		{
			if( ntrn_brush_cmd::execute() )
			{
				m_mtl->post_alphamap_change(m_brush.target_index, m_dirty_region);
				return true;
			}

			return false;
		}

		virtual bool undo(void)
		{
			nterrain_actor::ptr trn( m_trn );

			// terrain对象已经被释放？
			if( !trn )
				return false;

			// terrain对象的材质是否已经重建？
			if( trn->get_material() != m_mtl )
				return false;

			// 材质的alpha map是否已经重建？
			if( m_target != m_mtl->get_alpha_map(m_brush.target_index) )
				return false;

			// write back

			return true;
		}

	private:
		ntexture_splatting*	m_mtl;

	};

	//-----------------------------------------------------------------------------------------------
	// class nterrain_editor

	nterrain_editor::nterrain_editor(void):m_cursor_pos(0,0)
	{
	}

	nterrain_editor::~nterrain_editor(void)
	{
	}

	void nterrain_editor::bind_terrain(const nterrain_actor::ptr& trn_ptr)	
	{	
		m_trn_actor = trn_ptr; 
		_reset_brush_noise(m_brush.noise_freq, m_brush.strength);
	}

	void nterrain_editor::draw_widgets(nrender_primitive_draw_interface* PDI)
	{
		if( m_trn_actor.expired() )
			return;
		nterrain_actor::ptr trn(m_trn_actor);
		if( !trn )
			return;

		if( trn )
		{
			//-- 显示笔刷区域
			draw_brush_cursor(PDI);

			//-- 显示光标信息
			vector3 wp = trn->tile2world(m_cursor_pos.x, m_cursor_pos.y);
			std::wostringstream oss;
			oss << _T("(") << m_cursor_pos.x
				<< _T(",") << m_cursor_pos.y
				<< _T(")-h=") << trn->get_raw_height(m_cursor_pos.x, m_cursor_pos.y)
				<< _T(", world(") << wp.x
				<< _T(",") << wp.y
				<< _T(",") << wp.z
				<< _T(")")
				;
			PDI->draw_debug_string(2,2+16, oss.str(), color4ub(255,255,250,255));
		}
	}

	void nterrain_editor::on_mouse_left_down(const npoint& pt)
	{
		m_left_drag.begin_drag();
		_reset_brush_noise(m_brush.noise_freq, m_brush.strength);

		// 执行笔刷操作
		brush_paint(false);
	}

	void nterrain_editor::on_mouse_left_up()
	{
		m_left_drag.end_drag();
	}

	void nterrain_editor::on_mouse_right_down()
	{
		m_right_drag.begin_drag();

		_reset_brush_noise(m_brush.noise_freq, m_brush.strength);

		brush_paint(true);
	}

	void nterrain_editor::on_mouse_right_up()
	{
		m_right_drag.end_drag();
	}

	void nterrain_editor::on_mouse_wheel(int delta)
	{}

	void nterrain_editor::brush_paint(bool neg)
	{
		if( m_trn_actor.expired() )
			return;
		nterrain_actor::ptr trn(m_trn_actor);
		if( !trn )
			return;

		nterrain_brush my_brush = m_brush;
		
		// 计算笔刷强度值取反
		if( neg )
			my_brush.strength = -m_brush.strength;

		boost::shared_ptr<neditor_cmd> new_cmd;
		switch(my_brush.target)
		{
		case EBT_Heightmap:
			{
				boost::shared_ptr<ntrn_brush_cmd_hmap> cmd(new ntrn_brush_cmd_hmap);
				if( cmd->init(trn, trn->get_height_map(), my_brush, m_cursor_pos) )
				{
					if( cmd->execute() )
						new_cmd = boost::dynamic_pointer_cast<neditor_cmd>(cmd);
				}
			}
			break;
		case EBT_Alphamap:
			{
				boost::shared_ptr<ntrn_brush_cmd_alpha> cmd(new ntrn_brush_cmd_alpha);
				if( cmd->init(trn, my_brush, m_cursor_pos) )
				{
					if( cmd->execute() )
						new_cmd = boost::dynamic_pointer_cast<neditor_cmd>(cmd);
				}
			}
			break;
		case EBT_Decomap:
			break;
		}// end of switch

		
	}

	void nterrain_editor::on_mouse_move(const npoint& pt, bool ctrl_down, const ncamera* cam)
	{
		if( m_trn_actor.expired() )
			return;
		nterrain_actor::ptr trn(m_trn_actor);
		if( !trn )
			return;

		//-- update brush cursor pos
		ray mouse_ray;
		cam->get_mouse_ray(pt.x, pt.y, mouse_ray);

		const float CHECK_LENGTH = 1024;
		vector3 s = trn->get_scale();
		float scale = std::max(s.x, s.z);
		float len = CHECK_LENGTH*scale;
		vector3 end = mouse_ray.origin+mouse_ray.dir*len;

		ncheck_result chk;
		if( trn->line_check(chk, mouse_ray.origin, end) )
		{
			m_cursor_pos = trn->world2tile(chk.location.x, chk.location.z);
		}

		//-- 鼠标拖动操作
		if(m_left_drag.repeat())
		{
			brush_paint(false);
		}
		else if(m_right_drag.repeat())
		{
			brush_paint(true);
		}
	}

	void nterrain_editor::draw_brush_cursor(nrender_primitive_draw_interface* PDI)
	{
		switch(m_brush.shape)
		{
		case EBS_Circle:
			draw_terrain_circle(m_cursor_pos.x, m_cursor_pos.y, m_brush.inner_radius, PDI);
			draw_terrain_circle(m_cursor_pos.x, m_cursor_pos.y, m_brush.outer_radius, PDI);
			break;
		case EBS_Square:
			{
				int x = m_cursor_pos.x;
				int z = m_cursor_pos.y;
				int r = m_brush.inner_radius;
				draw_terrain_quad(x-r, x+r, z-r, z+r, PDI);
				r = m_brush.outer_radius;
				draw_terrain_quad(x-r, x+r, z-r, z+r, PDI);
			}
			break;
		default:
			nASSERT(0 && "unknown terrain brush shape.");
		}
	}

	void nterrain_editor::draw_terrain_quad(int left,int right,int top,int bottom, nrender_primitive_draw_interface* PDI)
	{
		if( m_trn_actor.expired() )
			return;
		nterrain_actor::ptr trn(m_trn_actor);
		if( !trn )
			return;

		_clip_rect(left, top, right, bottom, 
			trn->get_map_width(), trn->get_map_height());

		std::vector<vector3> line_vb;
		int x,y;

		//--left
		x = left;
		for(y=top; y<bottom; y++)
			line_vb.push_back(trn->tile2world(x,y));

		//--bottom
		y=bottom-1;
		for(x=left; x<right; x++)
			line_vb.push_back(trn->tile2world(x,y));

		//--right
		x=right-1;
		for(y=bottom-1; y>=top; y--)
			line_vb.push_back(trn->tile2world(x,y));

		//--top
		y=top;
		for(x=right-1; x>=left; x--)
			line_vb.push_back(trn->tile2world(x,y));

		for(size_t i=0; i<line_vb.size(); i++)
			line_vb[i].y += TRN_BRUSH_CURSOR_ZBIAS;
		PDI->draw_line_strip(line_vb, color4f(1,1,0,1));
	}

	void nterrain_editor::draw_terrain_circle(int centre_x, int centre_z, int radius, nrender_primitive_draw_interface* PDI)
	{
		if( m_trn_actor.expired() )
			return;
		nterrain_actor::ptr trn(m_trn_actor);
		if( !trn )
			return;

		vector3 trn_scale = trn->get_scale();
		float rx = radius*trn_scale.x;
		float rz = radius*trn_scale.z;

		vector3 center = trn->tile2world(centre_x, centre_z);
		float cx = center.x;
		float cz = center.z;

		std::vector<vector3> line_vb;

		vector3 start_pt(cx+rx, trn->get_height(cx+rx, cz), cz);
		line_vb.push_back(start_pt);

		vector3 v;
		int num_split =(int)std::min(64, int((2*nPI*rx)/trn_scale.x+4) );
		for(int i=1; i<num_split; i++)
		{
			float a = (nPI*2/num_split)*i;
			v.x = cx + cosf(a)*rx;
			v.z = cz + sinf(a)*rz;
			v.y = trn->get_height(v.x, v.z)+TRN_BRUSH_CURSOR_ZBIAS;
			line_vb.push_back(v);
		}

		line_vb.push_back(start_pt);

		PDI->draw_line_strip(line_vb, color4f(1,1,0,1));
	}

	void nterrain_editor::generate_noise(nrect rc, int numOctaves, float amplitude, float frequency)
	{
		if( m_trn_actor.expired() )
			return;
		nterrain_actor::ptr trn(m_trn_actor);
		if( !trn )
			return;

		// -1代表全部覆盖
		if( rc.right == -1 )
			rc.right = trn->get_map_width();
		if( rc.bottom == -1 )
			rc.bottom = trn->get_map_height();

		//-- 调用内部操作
		trn->generate_noise(rc, numOctaves, amplitude, frequency);
	}

	void nterrain_editor::import_heightmap(const nstring& img_file_name)
	{
		if( m_trn_actor.expired() )
			return;
		nterrain_actor::ptr trn(m_trn_actor);
		if( !trn )
			return;

		trn->import_heightmap(img_file_name);
	}

	void nterrain_editor::create_material_basic(const resource_location& texture_loc)
	{
		if( m_trn_actor.expired() )
			return;
		nterrain_actor::ptr trn(m_trn_actor);
		if( !trn )
			return;

		trn->create_material_basic(texture_loc);
	}

	void nterrain_editor::splat_set_layer(size_t layer_index, const resource_location& texture_loc, const vector2& uv_scale, float uv_rotate)
	{
		if( m_trn_actor.expired() )
			return;
		nterrain_actor::ptr trn(m_trn_actor);
		if( !trn )
			return;

		nmaterial_base* mtl_base_ptr = trn->get_material();
		ntexture_splatting* splat = ndynamic_cast<ntexture_splatting>(mtl_base_ptr);

		if( splat )
		{
			splat->set_layer_texture(layer_index, texture_loc);
			splat->set_layer_uv_param(layer_index, uv_scale, uv_rotate);
		}
	}

	void nterrain_editor::create_texture_splatting(size_t alpha_w, size_t alpha_h)
	{
		if( m_trn_actor.expired() )
			return;
		nterrain_actor::ptr trn(m_trn_actor);
		if( !trn )
			return;

		if( alpha_w == 0
			|| alpha_w == -1)
			alpha_w = trn->get_map_width()-1;
		if( alpha_h == 0
			|| alpha_h == -1)
			alpha_h = trn->get_map_height()-1;

		trn->create_texture_splatting(alpha_w, alpha_h);
	}

	void nterrain_editor::splat_layer_noise(size_t layer_index, nrect rc, int numOctaves, float amplitude, float frequency)
	{
		if( m_trn_actor.expired() )
			return;
		nterrain_actor::ptr trn(m_trn_actor);
		if( !trn )
			return;

		nmaterial_base* mtl_base_ptr = trn->get_material();
		ntexture_splatting* splat = ndynamic_cast<ntexture_splatting>(mtl_base_ptr);

		if( splat )
		{
			splat->generate_noise(layer_index, rc, numOctaves, amplitude, frequency);
		}
	}

	void nterrain_editor::get_layer_param(size_t layer_index, resource_location& out_tex, vector2& out_scale, float& out_rotate)
	{
		if( m_trn_actor.expired() )
			return;
		nterrain_actor::ptr trn(m_trn_actor);
		if( !trn )
			return;

		nmaterial_base* mtl_base_ptr = trn->get_material();
		ntexture_splatting* splat = ndynamic_cast<ntexture_splatting>(mtl_base_ptr);

		if( splat )
			splat->get_layer_param(layer_index, out_tex, out_scale, out_rotate);
	}

	bool nterrain_editor::empty() const
	{
		return m_trn_actor.expired();
	}
}//namespace nexus