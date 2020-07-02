#pragma once
#include "../nengine/engine_define.h"
#include "../nengine/mesh/nmesh_importer.h"
#include "../nengine/renderer/render_define.h"

#include "ActorXFile.h"

namespace nexus
{
	class PSA_importer
		: public nskeletal_anim_importer
	{
	public:
		PSA_importer(void);
		virtual ~PSA_importer(void);

		virtual nstring get_file_type() const	{	return _T(".PSA");}
		virtual nstring get_desc() const		{	return _T("ActorX Skeletal Animation");}
		virtual void clear()
		{
			m_file_data.clear();
		}

		virtual void import_from_file(const nstring& full_path, data& out_data);

	private:
		void process_skeleton(nskeleton_define& out_skel);
		nskeletal_anim_sequence::ptr process_sequence(const VAnimation& vanim);

	private:
		PSAFile	m_file_data;

		nDECLARE_CLASS(PSA_importer)
	};
}//namespace nexus