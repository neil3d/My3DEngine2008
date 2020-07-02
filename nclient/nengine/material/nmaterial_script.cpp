#include "StdAfx.h"
#include "nmaterial_script.h"
#include "../framework/nengine.h"

#define BOOST_REGEX_DYN_LINK
#include <boost/regex.hpp>

namespace nexus
{
	static enum ETransparentType _string_to_enum_trans(const std::string& str)
	{
		enum ETransparentType ret;

		if( str=="AlphaMasked" )
			ret = ETrans_AlphaMasked;
		else if( str=="Mixed" )
			ret = ETrans_Mixed;
		else if( str=="UnlitTrans" )
			ret = ETrans_UnlitTrans;
		else
			ret = ETrans_Opaque;
		
		return ret;
	}

	static enum EMaterialParameter _string_to_type(const std::string& type_str)
	{
		enum EMaterialParameter ret = EMPT_None;		

		std::string type_str_ci = boost::to_lower_copy(type_str);
		if(type_str_ci == "float")
			ret = EMPT_Float;
		else if(type_str_ci == "float2")
			ret = EMPT_Float2;
		else if(type_str_ci == "float3")
			ret = EMPT_Float3;
		else if(type_str_ci == "float4")
			ret = EMPT_Float4;
		else if(type_str_ci == "texture")
			ret = EMPT_Texture;
		else if(type_str_ci == "int")
			ret = EMPT_Int;

		return ret;
	}

	nmaterial_script::nmaterial_script(const nstring& name_str):nmaterial_template(name_str)
	{
	}

	nmaterial_script::~nmaterial_script(void)
	{
	}

	void nmaterial_script::load_from_file(const resource_location& loc)
	{
		m_location = loc;

		//--
		nfile_system* fs = nengine::instance()->get_file_sys();

		//-- 加载shader代码
		size_t file_size = fs->get_file_size(loc.pkg_name, loc.file_name);
		if(file_size > 0
			&& file_size !=-1)
		{
			m_shader_code.resize(file_size, 0);
			nfile::ptr file_ptr = fs->open_file(loc.pkg_name, loc.file_name, EFileRead);
			file_ptr->read_buffer((void*)m_shader_code.data(), file_size);
		}

		//-- 分析shader，取得所有参数的类型、名称
		parse_material_parameter_from_HLSL();
		prase_config_from_HLSL();
	}

	void nmaterial_script::prase_config_from_HLSL()
	{
		static boost::regex ex("//\\*[[:space:]]*"	//起始符号
			"([\\w]+)"	// key name
			"[[:space:]]*=[[:space:]]*([\\w]+)"		// value name
			"[;]"	// 分号结束
			, boost::regex::icase);	

		std::string::const_iterator start, end;
		start = m_shader_code.begin();
		end = m_shader_code.end();

		boost::match_results<std::string::const_iterator> what;
		boost::match_flag_type flags = boost::match_default;

		while( boost::regex_search(start, end, what, ex, flags) )
		{
			std::string key = what[1];
			std::string val_name = what[2];

			if( key == "TransparentType" )
				set_trans_type( _string_to_enum_trans(val_name) );
			

			// update search position: 
			start = what[0].second;
			// update flags: 
			flags |= boost::match_prev_avail; 
			flags |= boost::match_not_bob; 		
		}
	}

	void nmaterial_script::parse_material_parameter_from_HLSL()
	{
		//注意：HLSL定义变量的语法为：
		//	[Storage_Class] [Type_Modifier]	Type Name[Index] [: Semantic] [= Initializers] [Annotations] [: Register] [: Packoffset];
		// 为了简单起见，这里只识别“Type Name[: Semantic]”		
		
		static boost::regex ex("(int|texture|float[\\d]?)[[:space:]]+"	// 变量类型
			"([\\w]*)"	// 变量名
			"[[:space:]]?:[[:space:]]?(MTL_[\\w]+)"		// semantic
			"[;]"	// 分号结束
			, boost::regex::icase);	

		//todo: int	g_alphaMask:MTL_AlphaMask = 250; 这种默认值也要能识别出来

		std::string::const_iterator start, end;
		start = m_shader_code.begin();
		end = m_shader_code.end();

		boost::match_results<std::string::const_iterator> what;
		boost::match_flag_type flags = boost::match_default;

		while( boost::regex_search(start, end, what, ex, flags) )
		{
			material_parameter param;
			param.type = _string_to_type(what[1]);
			std::string var_name = what[2];
			param.name = what[3];	// take HLSL var sematic as material parameter name

			m_parameter_array.push_back(param);
			
			// update search position: 
			start = what[0].second;
			// update flags: 
			flags |= boost::match_prev_avail; 
			flags |= boost::match_not_bob; 		
		}
	}
}//namespace nexus