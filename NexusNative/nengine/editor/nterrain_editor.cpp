#include "StdAfx.h"
#include <ostream>
#include "nterrain_editor.h"
#include "neditor_engine.h"
#include "../renderer/nrender_element.h"
#include "terrain_brush.h"

namespace nexus
{

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

		bool _init(nterrain_actor::ptr trn, const nstring& target_map_name, const nterrain_brush& brush, npoint cursor_pos)
		{
			if( !trn || !trn->is_valid())
				return false;

			m_trn = trn;
			m_target_map_name = target_map_name;
			m_brush = brush;
			m_cursor_pos = cursor_pos;

			return true;
		}

		virtual bool execute(void)
		{
			nterrain_actor::ptr trn(m_trn);
			if( !trn || !trn->is_valid())
				return false;
			
			nheight_map<T>* target = get_target();
			if( !target )
				return false;
			
			//-- backup old heightmap			
			m_backup_region.left	= m_cursor_pos.x-m_brush.outer_radius;
			m_backup_region.right	= m_cursor_pos.x+m_brush.outer_radius;
			m_backup_region.top		= m_cursor_pos.y-m_brush.outer_radius;
			m_backup_region.bottom	= m_cursor_pos.y+m_brush.outer_radius;
			clip_rect(m_backup_region, target->get_width(), target->get_height());

			m_backup_map = target->create_sub_map(m_backup_region.left, m_backup_region.top,
				m_backup_region.get_width(), m_backup_region.get_height());


			//-- change heightmap
			switch(m_brush.op)
			{
			case EOP_Paint:
				{
					trn_brush_add<T> brush_op;
					m_dirty_region = do_terrain_brush(target,
						m_cursor_pos,
						m_brush,
						brush_op);
				}
				break;
			case EOP_Flatten:
				{
					trn_brush_average<T> avg;
					
					do_terrain_brush(target,
						m_cursor_pos,
						m_brush,
						avg);
					if( avg.get_count() > 0 )
					{
						trn_brush_apply_val<T> apply(avg.get_result());

						m_dirty_region = do_terrain_brush(target,
							m_cursor_pos,
							m_brush,
							apply);						
					}
				}
				break;
			case EOP_Smooth:
				{
					trn_brush_smooth<T> brush_op(5);
					m_dirty_region = do_terrain_brush(target,
						m_cursor_pos,
						m_brush,
						brush_op);				
					brush_op.apply(target);
				}
				break;
			case EOP_Apply:
				{
					trn_brush_apply_val<T> brush_op(m_brush.strength);
					m_dirty_region = do_terrain_brush(target,
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
					clip_rect(brush_rect, trn->get_map_width(), trn->get_map_height());

					trn_brush_noise<T> brush_op(brush_rect.get_width(), 
						brush_rect.get_height(),
						npoint(brush_rect.left, brush_rect.top)
						);
					m_dirty_region = do_terrain_brush(target,
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

			// 检测terrain对象已经被释放
			if( !trn || !trn->is_valid())
				return false;

			nheight_map<T>* target = get_target();
			if( !target )
				return false;

			// write data
			target->blit(m_backup_map.get(), m_backup_region.left, m_backup_region.top);			

			return true;
		}


		virtual bool redo(void)
		{
			return execute();
		}

		const nrect& get_dirty_region() const	{	return m_dirty_region; }
		virtual nheight_map<T>* get_target() const {	return NULL;}
	
	protected:
		boost::weak_ptr<nterrain_actor>	m_trn;
		nstring			m_target_map_name;
		nterrain_brush	m_brush;
		npoint			m_cursor_pos;

		shared_ptr<nheight_map<T> >	m_backup_map;
		nrect			m_backup_region;
		nrect			m_dirty_region;
	};

	//-----------------------------------------------------------------------------------------------
	/** 编辑修改地形的高度图 */
	class ntrn_brush_cmd_hmap : public ntrn_brush_cmd<unsigned short>
	{
	public:
		ntrn_brush_cmd_hmap()	{}
		virtual ~ntrn_brush_cmd_hmap()	{}

		bool init(nterrain_actor::ptr trn, const nterrain_brush& brush, npoint cursor_pos)
		{
			return _init(trn, _T("heightmap"), brush, cursor_pos);
		}

		virtual nheight_map<unsigned short>* get_target() const 
		{	
			nterrain_actor::ptr trn( m_trn );
			if( !trn || !trn->is_valid())
				return NULL;

			return trn->get_height_map();
		}

		virtual bool execute(void)
		{
			nterrain_actor::ptr trn( m_trn );
			if( ntrn_brush_cmd::execute() )
			{
				trn->post_heightmap_change(m_dirty_region);
				return true;
			}

			return false;
		}

		virtual bool undo(void)
		{			
			nterrain_actor::ptr trn( m_trn );
			if(ntrn_brush_cmd::undo())
			{
				trn->post_heightmap_change(m_backup_region);
				return true;
			}

			return true;
		}

		virtual nstring get_name(void)
		{
			return _T("terrain heightmap brush");
		}

		virtual nstring get_desc(void)
		{
			wostringstream oss;
			oss << get_name()
				<< _T("(")
				<< m_dirty_region.left << _T(",")
				<< m_dirty_region.top << _T(",")
				<< m_dirty_region.right << _T(",")
				<< m_dirty_region.bottom << _T(",")
				<< _T(")");
			return oss.str();
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
			if( !trn || !trn->is_valid())
				return false;
			
			nterrain_mtl_setup* mtl = trn->get_material();
			if( !mtl )
				return false;
			if( (size_t)brush.target_index >= mtl->get_num_layers() )
				return false;

			nstring layer_name = mtl->get_layer_name(brush.target_index);
			if( layer_name.empty() )
				return false;			

			//-- 调用基类功能
			return _init(trn, layer_name, brush, cursor_pos);			
		}

		virtual nheight_map<unsigned char>* get_target() const 
		{	
			nterrain_actor::ptr trn( m_trn );
			if( !trn || !trn->is_valid())
				return false;

			nterrain_mtl_setup* mtl = trn->get_material();
			nalpha_map::ptr amap = mtl->get_layer_alpha(m_target_map_name);
			
			return amap.get();
		}

		virtual bool execute(void)
		{
			nterrain_actor::ptr trn( m_trn );
			if( ntrn_brush_cmd::execute() )
			{
				nterrain_mtl_setup* mtl = trn->get_material();
				mtl->post_layer_alpha_change(m_target_map_name, m_dirty_region);
				return true;
			}

			return false;
		}	

		bool is_alphamap_valid() const
		{
			nterrain_actor::ptr trn( m_trn );
			if( !trn || !trn->is_valid())
				return false;

			nterrain_mtl_setup* mtl = trn->get_material();
			if( !mtl )
				return false;

			if( mtl->get_layer_index(m_target_map_name) == -1)
				return false;

			return true;
		}

		virtual bool undo(void)
		{
			if( !is_alphamap_valid() )
				return false;

			if(ntrn_brush_cmd::undo())
			{
				nterrain_actor::ptr trn( m_trn );
				nterrain_mtl_setup* mtl = trn->get_material();

				mtl->post_layer_alpha_change(m_target_map_name, m_backup_region);					
				return true;
			}

			return false;
		}

		virtual bool redo(void)
		{
			if( is_alphamap_valid() )				
				return execute();
			return false;
		}

		virtual nstring get_name(void)
		{
			return _T("terrain alphamap brush");
		}

		virtual nstring get_desc(void)
		{
			wostringstream oss;
			oss << get_name()
				<< _T(" layer ") << m_target_map_name
				<< _T(",(")
				<< m_dirty_region.left << _T(",")
				<< m_dirty_region.top << _T(",")
				<< m_dirty_region.right << _T(",")
				<< m_dirty_region.bottom << _T(",")
				<< _T(")");
			return oss.str();
		}
	};

	//-----------------------------------------------------------------------------------------------
	/** 编辑修改当前被选中的地形装饰层*/
	class ntrn_brush_cmd_deco : public neditor_cmd
	{
	protected:
		nterrain_deco_layer*	m_target_map;
		boost::weak_ptr<nterrain_actor>	m_trn;
		nstring			m_target_map_name;
		nterrain_brush	m_brush;
		npoint			m_cursor_pos;

		shared_ptr<nalpha_map>	m_backup_map;
		nrect			m_backup_region;
		nrect			m_dirty_region;
	public:
		ntrn_brush_cmd_deco()	{}
		virtual ~ntrn_brush_cmd_deco()	{}

		bool init(nterrain_actor::ptr trn, const nterrain_brush& brush, npoint _cursor_pos)
		{
			if( !trn || !trn->is_valid())
				return false;

			nterrain_deco_setup* mtl = trn->get_deco();
			if( !mtl )
				return false;
			if( brush.target_index >= (int)mtl->get_deco_layer_count() )
				return false;

			nstring layer_name = mtl->get_deco_layer(brush.target_index)->get_name();
			if( layer_name.empty() )
				layer_name=mtl->get_deco_layer(brush.target_index)->get_resource_loc().to_string();

			m_target_map=mtl->get_deco_layer(brush.target_index).get();

			//将笔刷的中心点从terrain坐标转换为分布图坐标
			uint32 m_terrain_width=trn->get_map_width();
			uint32 m_terrain_height=trn->get_map_height();
			uint32 m_terrain_max_size=__max(m_terrain_width,m_terrain_height);
			npoint cursor_pos;
			cursor_pos.x=(int)((_cursor_pos.x/(float)m_terrain_width)*m_target_map->get_grid_size());
			cursor_pos.y=(int)((_cursor_pos.y/(float)m_terrain_height)*m_target_map->get_grid_size());
			nterrain_brush new_brush=brush;
			//笔刷的半径也要从terrain空间转换到分布图空间
			new_brush.inner_radius=(int)((brush.inner_radius/(float)m_terrain_max_size)*m_target_map->get_grid_size());
			new_brush.outer_radius=(int)((brush.outer_radius/(float)m_terrain_max_size)*m_target_map->get_grid_size());
			
			m_trn = trn;
			m_target_map_name = layer_name;
			m_brush = brush;
			m_cursor_pos = cursor_pos;

			return true;
		}

		virtual nalpha_map* get_target() const 
		{	
			nterrain_actor::ptr trn( m_trn );
			if( !trn || !trn->is_valid())
				return 0;

			if(!is_alphamap_valid())
			{
				return 0;
			}

			nalpha_map::ptr amap = m_target_map->get_density_map();

			return amap.get();
		}

		virtual bool execute(void)
		{
			nterrain_actor::ptr trn(m_trn);
			if( !trn || !trn->is_valid())
				return false;

			nalpha_map* target = get_target();
			if( !target )
				return false;

			//-- backup old heightmap			
			m_backup_region.left	= m_cursor_pos.x-m_brush.outer_radius;
			m_backup_region.right	= m_cursor_pos.x+m_brush.outer_radius;
			m_backup_region.top		= m_cursor_pos.y-m_brush.outer_radius;
			m_backup_region.bottom	= m_cursor_pos.y+m_brush.outer_radius;
			clip_rect(m_backup_region, (long)target->get_width(), (long)target->get_height());

			m_backup_map = target->create_sub_map(m_backup_region.left, m_backup_region.top,
				m_backup_region.get_width(), m_backup_region.get_height());

			srand(m_target_map->get_random_seed());
			trn_brush_deco_rand brush_op(trn->get_map_width(),trn->get_map_height());
			m_dirty_region = do_brush(target,
						m_cursor_pos,
						m_brush,
						brush_op);

			nterrain_deco_setup* mtl = trn->get_deco();
			mtl->post_deco_map_changed(m_target_map, m_dirty_region);

			return true;
		}	

		bool is_alphamap_valid() const
		{
			nterrain_actor::ptr trn( m_trn );
			if( !trn || !trn->is_valid())
				return false;

			nterrain_deco_setup* mtl = trn->get_deco();
			if( !mtl )
				return false;

			for (size_t i=0;i<mtl->get_deco_layer_count();++i)
			{
				if(mtl->get_deco_layer(i).get()==m_target_map)
				{
					return true;
				}
			}

			return false;
		}

		virtual bool undo(void)
		{
			if( !is_alphamap_valid() )
				return false;

			nterrain_actor::ptr trn( m_trn );

			// 检测terrain对象已经被释放
			if( !trn || !trn->is_valid())
				return false;

			nalpha_map* target = get_target();
			if( !target )
				return false;

			// write data
			target->blit(m_backup_map.get(), m_backup_region.left, m_backup_region.top);
			
			nterrain_deco_setup* mtl = trn->get_deco();
			mtl->post_deco_map_changed(m_target_map, m_backup_region);					
			return true;
		}

		virtual bool redo(void)
		{
			if( is_alphamap_valid() )				
				return execute();
			return false;
		}

		virtual nstring get_name(void)
		{
			return _T("terrain alphamap brush");
		}

		virtual nstring get_desc(void)
		{
			wostringstream oss;
			oss << get_name()
				<< _T(" layer ") << m_target_map_name
				<< _T(",(")
				<< m_dirty_region.left << _T(",")
				<< m_dirty_region.top << _T(",")
				<< m_dirty_region.right << _T(",")
				<< m_dirty_region.bottom << _T(",")
				<< _T(")");
			return oss.str();
		}
	};

	//-----------------------------------------------------------------------------------------------
	// class nterrain_editor

	nterrain_editor::nterrain_editor(void):m_cursor_pos(0,0),
		m_show_chunk_edge(true)
	{
	}

	nterrain_editor::~nterrain_editor(void)
	{
	}

	void nterrain_editor::bind_terrain(const nterrain_actor::ptr& trn_ptr)	
	{	
		m_trn_actor = trn_ptr; 
		nbrush_noise::instance()->reset_brush_noise(m_brush.noise_freq, m_brush.strength);
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

			//-- 显示Chunk边缘
			if( m_show_chunk_edge )
			{
				int chunk_step = trn->get_chunk_size()-1;

				int yc = (trn->get_map_width()-1)/chunk_step;
				int xc = (trn->get_map_height()-1)/chunk_step;
				for(int y=0; y<yc; y++)
				{
					for(int x=0; x<xc; x++)
					{
						int px = x*chunk_step;
						int py = y*chunk_step;

						draw_terrain_quad(px, px+chunk_step+1, py, py+chunk_step+1, PDI);
					}
				}//end of for()
			}//end of if


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
		(void)pt;
		m_left_drag.begin_drag();
		nbrush_noise::instance()->reset_brush_noise(m_brush.noise_freq, m_brush.strength);

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

		nbrush_noise::instance()->reset_brush_noise(m_brush.noise_freq, m_brush.strength);

		brush_paint(true);
	}

	void nterrain_editor::on_mouse_right_up()
	{
		m_right_drag.end_drag();
	}

	void nterrain_editor::on_mouse_wheel(int delta)
	{
		(void)delta;
	}

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
				if( cmd->init(trn, my_brush, m_cursor_pos) )
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
			{
				boost::shared_ptr<ntrn_brush_cmd_deco> cmd(new ntrn_brush_cmd_deco);
				if( cmd->init(trn, my_brush, m_cursor_pos) )
				{
					if( cmd->execute() )
						new_cmd = boost::dynamic_pointer_cast<neditor_cmd>(cmd);
				}
			}
			break;

		}// end of switch

		//-- 把command交给enditor engine管理
		if( new_cmd )
			neditor_engine::instance()->push_command(new_cmd);
		
	}

	void nterrain_editor::on_mouse_move(const npoint& pt, bool ctrl_down, const ncamera* cam)
	{
		(void)ctrl_down;

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
		if( trn->line_check(chk, mouse_ray.origin, end, ELCT_Terrain) )
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

		clip_rect(left, top, right, bottom, 
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

	bool nterrain_editor::empty() const
	{
		return m_trn_actor.expired();
	}

	nterrain_mtl_setup* nterrain_editor::get_material()
	{
		if( m_trn_actor.expired() )
			return NULL;
		nterrain_actor::ptr trn(m_trn_actor);
		if( !trn )
			return NULL;
		return trn->get_material();
	}
}//namespace nexus