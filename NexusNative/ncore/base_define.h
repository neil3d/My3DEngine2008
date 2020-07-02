/**
*	nexus core - base define
*
*	Author: Neil
*	Date:	Sept, 2008
*/

#ifndef _BASE_DEFINE_H
#define _BASE_DEFINE_H

namespace nexus
{
	#ifdef WIN32
		#define DLLEXPORT			__declspec( dllexport )
		#define DLLIMPORT			__declspec( dllimport )
	#endif //WIN32


	#ifdef NCORE_EXPORTS
		#define nCoreAPI DLLEXPORT
	#else
		#define nCoreAPI DLLIMPORT
	#endif

		// 禁止使用拷贝极造函数和赋值操作的宏
		// 应在类的private:中使用 
	#define DISALLOW_COPY_AND_ASSIGN(TypeName)	\
	private:								\
		TypeName(const TypeName&);				\
		void operator=(const TypeName&);


}//namespace nexus
#endif //_BASE_DEFINE_H