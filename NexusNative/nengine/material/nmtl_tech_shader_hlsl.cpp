#include "StdAfx.h"
#define BOOST_REGEX_DYN_LINK
#include <boost/regex.hpp>
#include "nmtl_tech_shader_hlsl.h"
#include "../framework/nengine.h"
#include "../resource/nresource_manager.h"
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define NOMINMAX
#include <Windows.h>
#endif
namespace nexus
{
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

	nmtl_tech_shader_hlsl::nmtl_tech_shader_hlsl(const nstring& name_str):nmtl_tech_shader(name_str)
	{
	}

	nmtl_tech_shader_hlsl::~nmtl_tech_shader_hlsl(void)
	{
	}

	void nmtl_tech_shader_hlsl::load_from_file(const resource_location& loc)
	{		
		boost::mutex::scoped_lock lock(m_mutex);
		//--
		nfile_system* fs = nengine::instance()->get_file_sys();

		//-- 加载shader代码
		size_t file_size = fs->get_file_size(loc.pkg_name, loc.file_name);
		if(file_size > 0
			&& file_size !=-1)
		{
			m_shader_code.resize(file_size+1, 0);
			nfile::ptr file_ptr = fs->open_file(loc.pkg_name, loc.file_name, EFileRead);
			file_ptr->read_buffer((void*)m_shader_code.data(), file_size);
		}
		else
		{
			nthrow2(_T("load material hlsl shader failed"), loc.to_string().c_str());
		}

		//-- 分析shader，取得所有参数的类型、名称
		parse_parameter_from_hlsl();
		
		m_location = loc;

		nstring append_name = _T("");

		::InterlockedExchange(&m_ready, 1L);
	}

	void nmtl_tech_shader_hlsl::parse_parameter_from_hlsl()
	{
		//注意：HLSL定义变量的语法为：
		//	[Storage_Class] [Type_Modifier]	Type Name[Index] [: Semantic] [= Initializers] [Annotations] [: Register] [: Packoffset];
		// 为了简单起见，这里只识别“Type Name[: Semantic]”		

		//-- 先分析数值型参数
		{
			static boost::regex ex("(int|float[\\d]?)[[:space:]]+"	// 变量类型
				"([\\w]*)"	// 变量名
				"[[:space:]]?:[[:space:]]?(MTL_[\\w]+)"		// semantic
				"[\\s]?(<(.+?)>;)?"	// Annotation
				//-- 为了匹配默认值,所以取得结束,例如: int	g_alphaMask:MTL_AlphaMask = 250; 
				//"[;]"	// 分号结束 
				, boost::regex::icase);	

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

				string test1 =  what[4];
				string test2 =  what[5];
				// 指定默认资源
				boost::sub_match<std::string::const_iterator> anno_match = what[4];				
				if( anno_match.matched )
				{
					std::string anno_str = anno_match;
					parse_annotation(anno_str, param);					
				}

				m_param_array.push_back(param);

				// update search position: 
				start = what[0].second;
				// update flags: 
				flags |= boost::match_prev_avail; 
				flags |= boost::match_not_bob; 		
			}
		}

		//-- 再分析贴图型参数，支持Annotation指定默认资源
		{
			static boost::regex ex("(texture)[[:space:]]+"	// 变量类型
				"([\\w]*)"	// 变量名
				"[[:space:]]?:[[:space:]]?(MTL_[\\w]+)"		// semantic
				"[\\s]?(<(.+?)>;)?"	// Annotation
				//-- 为了匹配默认值,所以取得结束,例如: int	g_alphaMask:MTL_AlphaMask = 250; 
				//"[;]"	// 分号结束 
				, boost::regex::icase);	

			std::string::const_iterator start, end;
			start = m_shader_code.begin();
			end = m_shader_code.end();

			boost::match_results<std::string::const_iterator> what;
			boost::match_flag_type flags = boost::match_default;

			while( boost::regex_search(start, end, what, ex, flags) )
			{			
				material_parameter param;
				param.type = EMPT_Texture;
				std::string var_name = what[2];
				param.name = what[3];	// take HLSL var semantic as material parameter name				

				// 指定默认资源
				boost::sub_match<std::string::const_iterator> anno_match = what[5];				
				if( anno_match.matched )
				{
					std::string anno_str = anno_match;
					parse_annotation(anno_str, param);					
				}

				m_param_array.push_back(param);

				// update search position: 
				start = what[0].second;
				// update flags: 
				flags |= boost::match_prev_avail; 
				flags |= boost::match_not_bob; 		
			}
		}
	}

	void nmtl_tech_shader_hlsl::parse_annotation(const std::string text, material_parameter& param)
	{
		static boost::regex ex("string[\\s]"
			"([\\w]*)"	// 变量名
			"[\\s]=[\\s]"
			"\\\"([^\\s]*)\\\";"
			, boost::regex::icase);	

		std::string::const_iterator start, end;
		start = text.begin();
		end = text.end();

		boost::match_results<std::string::const_iterator> what;
		boost::match_flag_type flags = boost::match_default;

		while( boost::regex_search(start, end, what, ex, flags) )
		{
			boost::sub_match<std::string::const_iterator> key_match = what[1];				
			boost::sub_match<std::string::const_iterator> val_match = what[2];				
			
			if( key_match.matched
				&& val_match.matched )
			{
				std::string key = what[1];
				std::string val = what[2];

				if(key == "texture2d")
				{
					resource_location loc( conv_string(val) );
					param.texture_type = 0;
					param.tex_value = nresource_manager::instance()->load_texture_2d(loc, EIO_Block);
				}
				else if(key == "cubemap")
				{
					resource_location loc( conv_string(val) );
					param.texture_type = 1;
					param.tex_value = nresource_manager::instance()->load_cube_map(loc, EIO_Block);
				}
				else if(key == "macro")
				{
					param.macro = val;
				}
				else if(key == "value")
				{
					stringstream ss(val);
					string substring;
					vector<float> values;
					while( getline(ss,substring,','))
					{
						values.push_back(atof(substring.c_str())); 
					}

					if (!values.empty())
					{
						if (param.type == EMPT_Int)
						{
							param.int_value = (int)values[0];
						} 
						else if(param.type == EMPT_Float )
						{
							param.vec_value.x = values[0];
						}
						else if(param.type == EMPT_Float2 )
						{
							param.vec_value.x = values[0];
							param.vec_value.y = values[1];
						}
						else if(param.type == EMPT_Float3 )
						{
							param.vec_value.x = values[0];
							param.vec_value.y = values[1];
							param.vec_value.z = values[2];
						}
						else if(param.type == EMPT_Float4 )
						{
							param.vec_value.x = values[0];
							param.vec_value.y = values[1];
							param.vec_value.z = values[2];
							param.vec_value.w = values[3];
						}
					}					
				}
			}
		
			// update search position: 
			start = what[0].second;
			// update flags: 
			flags |= boost::match_prev_avail; 
			flags |= boost::match_not_bob; 		
		}
	}
}//namespace nexus