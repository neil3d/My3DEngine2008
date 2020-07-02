#ifndef _NEXUS_F3D_VERTEX_ANIM_IMPORTER_H_
#define _NEXUS_F3D_VERTEX_ANIM_IMPORTER_H_

#include "../nengine/engine_define.h"
#include "../nengine/mesh/nmesh_importer.h"
#include "../nengine/renderer/render_define.h"

namespace nexus
{
	typedef std::map<DWORD, nanim_mesh_sequence::ptr>	anim_mesh_seq_map;
	class F3D_vertex_anim_importer :
		public nanim_mesh_importer
	{
	public:
		F3D_vertex_anim_importer(void);
		virtual ~F3D_vertex_anim_importer(void);

		virtual nstring get_file_type() const	{	return _T(".fak");}		
		virtual nstring get_desc() const		{	return _T("F3D Animation Mesh");}
		virtual nanim_mesh::ptr import_from_file(const nstring& full_path);
		virtual void clear() 
		{
			m_seq_map.clear();
		}
	private:
		anim_mesh_seq_map	m_seq_map;

		nDECLARE_CLASS(F3D_vertex_anim_importer)
	};
}//namespace nexus

#endif //_NEXUS_F3D_VERTEX_ANIM_IMPORTER_H_