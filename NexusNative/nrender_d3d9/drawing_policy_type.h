/**
*	Nexus Engine - drawing policy typeπ‹¿Ì
*
*	Author: Neil
*	Date:	Oct, 2008
*/


#ifndef _NEXUS_D3D9_DRAWING_POLICY_TYPE
#define _NEXUS_D3D9_DRAWING_POLICY_TYPE

namespace nexus
{
	struct drawing_policy_type
	{
		typedef shared_ptr<drawing_policy_type> ptr;

		nname				dp_name;
		std::string			shader_source;
		shader_macro_array	macros;

		drawing_policy_type(const nstring& _dp_name):dp_name(_dp_name)
		{}
		virtual ~drawing_policy_type(void)
		{}

		virtual bool get_include_code(const char* include_name, std::string& out_code)
		{	return false; }

		virtual void load_shader_file();
		nstring get_name_str() const	{	return dp_name.name_str;}
	};

	typedef std::map<nname, drawing_policy_type::ptr>	dp_type_map;
	/**
	 *	drawing policy type list
	*/
	class dp_type_list
	{
	public:
		dp_type_map	m_dp_type_map;
	public:
		dp_type_list(void);
		~dp_type_list(void);

		void create_all_type();
		void add_drawing_policy_type(drawing_policy_type::ptr dp_ptr);
		drawing_policy_type::ptr find_drawing_policy_type(const nstring& dp_name_str);
		void clear();
	};
}//namespace nexus
#endif //_NEXUS_D3D9_DRAWING_POLICY_TYPE