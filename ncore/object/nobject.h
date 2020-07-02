/**
*	Core of Nexus Engine -  Core Object
*
*	Author: Neil
*	Date:	Aug, 2008
*/

#ifndef _NOBJECT_H_
#define _NOBJECT_H_
#include "../base_define.h"
#include "nclass.h"
#include "nobject_alloc.h"

namespace nexus
{
	class narchive;

	enum attribute_flag
	{
		attribute_editable	= 1 << 0,
		attribute_editconst	= 1 << 1,
	};

	/**
	 *	Nexus Engine对象模型的最基类
	 *
	 *	@remark	扩展标准C++,提供高级RTTI, 内存管理, 反射, 序列化等功能;
	 *			nonecopyale;
	*/
	class nCoreAPI nobject : public nobject_alloc
	{
	public:
		nobject(void);
		virtual ~nobject(void);

		virtual void serialize(narchive& ar);		
	private:
		//-- none copyable
		const nobject& operator = (const nobject& other);
		nobject(const nobject& other);

	public:
		//-- reflection
		static	nclass		m_class_nobject;
		virtual nclass*		reflection_get_class() const;
		static	nclass*		reflection_get_static_class();
		static	nobject*	reflection_alloc_object();
	};

#define nDECLARE_VIRTUAL_CLASS(ClassName)\
public:\
	static nclass	m_class_##ClassName;\
	virtual nclass* reflection_get_class() const;\
	static	nclass*	reflection_get_static_class();

#define nDEFINE_VIRTUAL_CLASS(ClassName, SuperClass)\
	nclass ClassName::m_class_##ClassName(_T(#ClassName), &SuperClass::m_class_##SuperClass, NULL);\
	nclass* ClassName::reflection_get_class() const	{	return &m_class_##ClassName;}\
	nclass*	ClassName::reflection_get_static_class(){	return &m_class_##ClassName;}

#define nDECLARE_CLASS(ClassName)\
public:\
	static nclass	m_class_##ClassName;\
	virtual nclass* reflection_get_class() const;\
	static nobject* reflection_alloc_object();\
	static	nclass*	reflection_get_static_class();

#define nDEFINE_CLASS(ClassName, SuperClass)\
	nclass ClassName::m_class_##ClassName(_T(#ClassName), &SuperClass::m_class_##SuperClass, ClassName::reflection_alloc_object);\
	nclass* ClassName::reflection_get_class() const		{	return &m_class_##ClassName;}\
	nobject* ClassName::reflection_alloc_object()		{	return nNew ClassName; }\
	nclass*	ClassName::reflection_get_static_class()	{	return &m_class_##ClassName;}

#define nDECLARE_NAMED_CLASS(ClassName)\
public:\
	static nclass	m_class_##ClassName;\
	virtual nclass* reflection_get_class() const;\
	static nobject* reflection_alloc_named_object(const nstring&);\
	static	nclass*	reflection_get_static_class();

#define nDEFINE_NAMED_CLASS(ClassName, SuperClass)\
	nclass ClassName::m_class_##ClassName(_T(#ClassName), &SuperClass::m_class_##SuperClass, ClassName::reflection_alloc_named_object, true);\
	nclass* ClassName::reflection_get_class() const		{	return &m_class_##ClassName;}\
	nobject* ClassName::reflection_alloc_named_object(const nstring& obj_name)		{	return nNew ClassName(obj_name); }\
	nclass*	ClassName::reflection_get_static_class()	{	return &m_class_##ClassName;}

}//namespace nexus
#endif //_NOBJECT_H_