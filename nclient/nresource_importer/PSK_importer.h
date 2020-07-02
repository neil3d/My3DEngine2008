#pragma once
#include "../nengine/engine_define.h"
#include "../nengine/mesh/nmesh_importer.h"
#include "../nengine/renderer/render_define.h"

#include "ActorXFile.h"

namespace nexus
{
	struct bone_inf
	{
		unsigned int	bone_index;
		float			weight;
	};

	struct skin_vertex
	{
		vector3	pos;
		vector3	normal;
		vector3	tangent;
		vector2	uv;
		std::vector<bone_inf>	inf_array;
	};

	class PSK_importer
		: public nskeletal_mesh_importer
	{
	public:
		PSK_importer(void);
		virtual ~PSK_importer(void);

		virtual nstring get_file_type() const	{	return _T(".PSK");}
		virtual nstring get_desc() const		{	return _T("ActorX Skin Mesh");}
		virtual void clear()
		{
			m_file_data.clear();
		}

		virtual nskeletal_mesh::ptr import_from_file(const nstring& full_path);

	private:
		PSKFile	m_file_data;

		std::vector<skin_vertex>	m_vertices;

		nDECLARE_CLASS(PSK_importer)
	};
}//namespace nexus