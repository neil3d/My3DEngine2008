#ifndef _NEXUS_MTL_PARAM_SERIALIZE_H_
#define _NEXUS_MTL_PARAM_SERIALIZE_H_

namespace nexus
{
	template<>
	inline narchive& nserialize(narchive& ar, material_parameter& data, const TCHAR* obj_name)
	{
		nstring class_name(_T("material_parameter"));
		ar.object_begin(obj_name, class_name);

		nserialize(ar, data.name, _T("name"));
		nserialize_enum_as_int(ar, data.type, _T("type"));	
		nserialize(ar, data.int_value, _T("int_value"));
		nserialize(ar, data.vec_value, _T("vec_value"));
		nserialize(ar, data.texture_type, _T("texture_type"));
		nserialize(ar, data.macro, _T("macro"));

		resource_location tex_loc;
		if( ar.is_loading() )
		{
			nSERIALIZE(ar, tex_loc);
			if(tex_loc.is_valid())
			{
				switch (data.texture_type)
				{
				case 0:
					data.tex_value = nresource_manager::instance()->load_texture_2d(tex_loc, EIO_Block);
					break;
				case 1:
					data.tex_value = nresource_manager::instance()->load_cube_map(tex_loc, EIO_Block);
					break;
				default:
					nASSERT( 0 && "Unknow texture type for material parameter");
				}				
			}// end of if
		}
		else
		{
			if( data.tex_value )
				tex_loc = data.tex_value->get_location();
			nSERIALIZE(ar, tex_loc);
		}

		ar.object_end();

		return ar;
	}
}//namespace nexus

#endif