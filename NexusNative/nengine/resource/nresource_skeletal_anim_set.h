#ifndef _NEXUS_RESOURCE_SKELETAL_ANIM_SET_H_
#define _NEXUS_RESOURCE_SKELETAL_ANIM_SET_H_

#include "nresource.h"
#include "../mesh/skeletal_anim.h"

namespace nexus
{
	/**
	 *	一组骨骼动画
	 *	@remark 包括一个animation sequence数组，一个skeletal define
	*/
	class nAPI nresource_skeletal_anim_set :
		public nresource
	{
	public:
		typedef boost::intrusive_ptr<nresource_skeletal_anim_set> ptr;

		explicit nresource_skeletal_anim_set(const nstring& name_str);
		virtual ~nresource_skeletal_anim_set(void);

		void import_anim_sequence(const nstring& full_path);

		virtual bool ready() const	{	return true;}
		virtual void serialize(narchive& ar);

		size_t get_num_anim() const	{	return m_anim_array.size();}
		nskeletal_anim_sequence::ptr get_anim(size_t i) const	{	return m_anim_array[i];}
		nskeletal_anim_sequence::ptr find_anim_by_name(const nstring& name_str) const;

		const nskeleton_define& get_skeleton() const	{	return m_skeleton;}

	private:
		typedef std::vector<nskeletal_anim_sequence::ptr> sequence_array;

		nskeleton_define	m_skeleton;
		sequence_array		m_anim_array;
	};
}//namespace nexus

#endif //_NEXUS_RESOURCE_SKELETAL_ANIM_SET_H_