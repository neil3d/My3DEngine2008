#ifndef _NEXUS_AVATAR_SKELETAL_MESH_H_
#define _NEXUS_AVATAR_SKELETAL_MESH_H_
#include "nmesh_component.h"
#include "../resource/nresource_skeletal_mesh.h"
#include "../renderer/ncpu_skin_mesh.h"
#include "../renderer/nrender_element.h"
#include "../animation/nskel_anim_controller.h"
#include "nskeletal_mesh_component.h"

namespace nexus
{
	/**
	 *	换装系统专用的skeletal mesh组件
	 *	@remarks 与“nskeletal_mesh_component”的不同点在于：1共享navator_actor管理的一个动画控制器；2处理资源替换功能；
	 *	@see class nskeletal_mesh_component
	*/
	class navatar_skeletal_mesh
		: public nskeletal_mesh_component
	{
	public:
		navatar_skeletal_mesh(const nstring& name_str);
		virtual ~navatar_skeletal_mesh(void);

		virtual void update(float delta_time, const nviewport& view);

	protected:


		nDECLARE_NAMED_CLASS(navatar_skeletal_mesh)
	};
}//namespace nexus
#endif