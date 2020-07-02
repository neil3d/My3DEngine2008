/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Nov, 2008
*/

#ifndef _NEXUS_STATIC_MESH_IMPORTER_H_
#define _NEXUS_STATIC_MESH_IMPORTER_H_
#include "ncore.h"
#include "mesh_lod.h"
#include "anim_mesh.h"
#include "skeletal_mesh.h"
#include "skeletal_anim.h"

namespace nexus
{
	class nAPI nmesh_exporter
		: public nobject
	{
	public:
		nmesh_exporter(void)	{}
		virtual ~nmesh_exporter(void)	{}

		virtual nstring get_file_type() const = 0;
		virtual nstring get_desc() const = 0;
		virtual void clear() = 0;

		nDECLARE_VIRTUAL_CLASS(nmesh_exporter);
	};

	/**
	*	导入常用静态模型格式的接口基类
	*	@remark 使用外部插件实现
	*/
	class nAPI nstatic_mesh_importer
		: public nmesh_exporter
	{
	public:
		typedef shared_ptr<nstatic_mesh_importer> ptr;

		nstatic_mesh_importer(void)	{	}
		virtual ~nstatic_mesh_importer(void)	{	}

		virtual nstatic_mesh_lod::ptr import_from_file(const nstring& full_path) = 0;
		nDECLARE_VIRTUAL_CLASS(nstatic_mesh_importer);
	};

	class nAPI nanim_mesh_importer
		: public nmesh_exporter
	{
	public:
		typedef shared_ptr<nanim_mesh_importer> ptr;

		nanim_mesh_importer(void)	{	}
		virtual ~nanim_mesh_importer(void)	{	}

		virtual nanim_mesh::ptr import_from_file(const nstring& full_path) = 0;

		nDECLARE_VIRTUAL_CLASS(nanim_mesh_importer);
	};

	class nAPI nskeletal_mesh_importer
		: public nmesh_exporter
	{
	public:
		typedef shared_ptr<nskeletal_mesh_importer> ptr;

		nskeletal_mesh_importer(void)	{	}
		virtual ~nskeletal_mesh_importer(void)	{	}

		virtual nskeletal_mesh::ptr import_from_file(const nstring& full_path) = 0;

		nDECLARE_VIRTUAL_CLASS(nskeletal_mesh_importer);
	};

	class nAPI nskeletal_anim_importer
		: public nmesh_exporter
	{
	public:
		typedef shared_ptr<nskeletal_anim_importer> ptr;

		struct data
		{
			nskeleton_define	skeleton;
			std::vector<nskeletal_anim_sequence::ptr>	anim_array;
		};

		nskeletal_anim_importer(void)	{	}
		virtual ~nskeletal_anim_importer(void)	{	}

		virtual void import_from_file(const nstring& full_path, data& out_data) = 0;

		nDECLARE_VIRTUAL_CLASS(nskeletal_anim_importer);
	};
}//namespace nexus
#endif //_NEXUS_STATIC_MESH_IMPORTER_H_