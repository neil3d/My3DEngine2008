#ifndef _NEXUS_DYNAMIC_VB_POOL_
#define _NEXUS_DYNAMIC_VB_POOL_
#include "d3d_ptr.h"

namespace nexus
{
	/**
	 *	动态vertex buffer池
	 *	@remark 这个池子有什么意义吗？假想场景中有100个dynamic mesh，如果简单的为每个分配自己所需的VB，
	 *		那么所需的显存量为100个dynamic mesh的总和；如果每个dynamic mesh在每帧中：确认可见时，向池子
	 *		申请显存，在渲染完成后释放显存，那么所需的显存量为可见的dynamic mesh的峰值的和，对于一个较大
	 *		的场景来说，通常是远远小于所有mesh总和的；
	 *		可以用于GPU morph动画mesh，CPU粒子特效mesh等；
	*/
	class dynamic_vb_pool
	{
		dynamic_vb_pool(void);
		~dynamic_vb_pool(void);
	public:

		static dynamic_vb_pool* instance();

		void init(size_t max_item_size=512*1024)
		{
			m_max_item_size = max_item_size;
		}
		void destroy()
		{
			m_item_map.clear();
		}

		d3d_vb_ptr alloc_for_vertex_stream(const vertex_stream* stream, bool init_copy);
		void free(d3d_vb_ptr vb_ptr);

	private:
		d3d_vb_ptr pool_alloc(size_t s);

	private:
		size_t					m_max_item_size;		
		
		struct pool_item
		{
			d3d_vb_ptr	vb;
			size_t		size;
			bool		in_use;
			size_t		use_count;
		};
		typedef std::map<size_t, pool_item> item_map;
		item_map	m_item_map;	//key == pool_item.size
		size_t		m_pool_size;
	};
}//namespace nexus

#endif //_NEXUS_DYNAMIC_VB_POOL_