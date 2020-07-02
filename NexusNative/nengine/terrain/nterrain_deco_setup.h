#ifndef _NEXUS_TERRAIN_DECO_SETUP_H_
#define _NEXUS_TERRAIN_DECO_SETUP_H_

#include "ncore.h"
#include "../resource/nresource.h"
#include "nterrain_deco_chunk_base.h"

namespace nexus
{
	class nterrain_actor;

	class nAPI nterrain_deco_setup:public nobject
	{
	public:
		typedef boost::shared_ptr<nterrain_deco_setup> ptr;

		//	清空内部数据，并绑定父对象
		void	reset(nterrain_actor* owner);

		// 创建chunks
		void	create_chunks();

		// 装饰层相关接口
		size_t	get_deco_layer_count()	{return m_deco_layers.size();}

		boost::shared_ptr<nterrain_deco_layer>	get_deco_layer(size_t index);
		boost::shared_ptr<nterrain_deco_layer>	get_deco_layer(const nstring& name);

		boost::shared_ptr<nterrain_deco_layer>	add_deco_layer(deco_layer_type type,size_t size);
		boost::shared_ptr<nterrain_deco_layer>	add_deco_layer(deco_layer_type type,const nstring& name,size_t size);

		void	del_deco_layer(boost::shared_ptr<nterrain_deco_layer> layer);
		void	del_deco_layer(int index);

		void	post_deco_map_changed(nterrain_deco_layer* layer,const nrect& region);

		//序列化
		virtual void	serialize(narchive& ar);
		//==============================================================================================================================
	private:
		nterrain_actor*	m_terrain;

		typedef std::vector<boost::shared_ptr<nterrain_deco_layer> > st_deco_layers;
		st_deco_layers		m_deco_layers;
		//==============================================================================================================================
	public:
		//==============================================================================================================================
		nDECLARE_CLASS(nterrain_deco_setup)
		nterrain_deco_setup();
		~nterrain_deco_setup();
	};
};

#endif