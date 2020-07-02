/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _RENDER_DEFINE_H_
#define _RENDER_DEFINE_H_

namespace nexus
{
	typedef unsigned int hit_id;

	enum ERenderMode
	{
		ERM_Wireframe,
		ERM_Unlit,
		ERM_Lit,
		ERM_LightingOnly,
	};


	// 将底层的绘制策略枚举出来，供材质绑定Tech Usage使用
	enum EDrawingPolicy
	{
		EDP_PreDepth	,	// 绘制出Z缓冲
		EDP_BaseColor	,	// 绘制出自发光，LightMap静态状态，以及非光照模式材质
		EDP_Unlit			,	// 非光照模式渲染
		EDP_Wireframe	,	// 绘制线框模式
		EDP_Point_Light				,	//
		EDP_Spot_Light					,	//
		EDP_Directional_Light		,	//
		EDP_Hit_Proxy,
		EDP_Num	
	};

	enum EDepthGroup
	{
		EDG_Background = 0,	// 背景组
		EDG_WorldGeometry,	// 世界场景Mesh组，使用简单的透明分类渲染策略
		EDG_WorldActor,		// 世界对象组，使用复杂的透明分类渲染策略
		EDG_Foreground,		// 前景组
		EDG_Max,
	};

	
	enum EBlendType
	{
		EOpaque = 0,	// 完全不透明	
		ETranslucent,	//完全透明,alpha混合
		EAdditive,			//相加混合
		EMax
	};

	struct render_config
	{
		void*	window_handle;
		size_t	width,
				height;
		size_t	color_bits;		
		bool	bWindowed;
		bool	bEnableHDR;
		bool bEnableSSAO;
	};

	enum EPrimitiveType
	{
		EDraw_None = 0,
		EDraw_PointList,
		EDraw_LineList,
		EDraw_LineStrip,
		EDraw_TriangleList,
		EDraw_TriangleStrip,
		EDraw_TriangleFan,
	};

	struct shader_define
	{
		std::string	name;
		std::string	value;

		shader_define(void)	{}
		shader_define(const std::string& _name, const std::string& _value):name(_name),value(_value)
		{}
	};
	typedef std::vector<shader_define> shader_macro_array;

	/**
	*	just a interface holder
	*/
	struct nshader_modifier
	{
	public:
		nshader_modifier()
		{
			m_macro_array.reserve(8);
			m_name.reserve(32);
		}

		void clear()
		{
			m_macro_array.clear();
			m_name.clear();
		}

		void add_macro(const shader_define& marco)
		{
			m_macro_array.push_back(marco);
			m_name.append(conv_string(marco.name));
		}

		const std::vector<shader_define>& get_macro_array() const
		{
			return m_macro_array;
		}

		const nstring& get_name() const
		{
			return m_name;
		}

		const nshader_modifier& operator += (const nshader_modifier& v)
		{
			if (!v.get_name().empty())
			{
				m_macro_array.insert(m_macro_array.end(),v.get_macro_array().begin(),v.get_macro_array().end());
				m_name += v.get_name();
			}
			
			return *this;
		}

	protected:
		std::vector<shader_define> m_macro_array;
		nstring m_name;
	};

	template<>
	inline narchive& nserialize(narchive& ar, shader_define& d, const TCHAR* obj_name)
	{
		nstring class_name(_T("shader_define"));
		ar.object_begin(obj_name, class_name);
		nserialize(ar, d.name, _T("name"));
		nserialize(ar, d.value, _T("value"));		
		ar.object_end();
		return ar;
	}

}//namespace nexus

#endif //_RENDER_DEFINE_H_