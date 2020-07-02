#ifndef _NEXUS_AVATAR_ATTACHMENT_H_
#define _NEXUS_AVATAR_ATTACHMENT_H_
#include "nstatic_mesh_component.h"
#include "../mesh/skeletal_anim.h"

namespace nexus
{
	/**
	 *	专门用来做换装挂载的静态mesh组件
	 *	@remarks 在基类的基础上增加两个功能，1根据指定的骨骼自动计算world矩阵；2处理异步IO资源替换，避免短暂消失现象；
	*/
	class navatar_attachment
		: public nstatic_mesh_component
	{
	public:
		navatar_attachment(const nstring& name_str);
		virtual ~navatar_attachment(void);

		void bind_to_bone(const nstring& bone_name, const nskeleton_define* skel);
		virtual void update(float delta_time, const nviewport& view);

	protected:
		nstring		m_bone_name;
		int			m_bone_index;
		matrix44	m_ref_base;

		nDECLARE_NAMED_CLASS(navatar_attachment)
	};
}//namespace nexus

#endif //_NEXUS_AVATAR_ATTACHMENT_H_