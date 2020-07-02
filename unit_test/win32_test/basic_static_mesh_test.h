/**
*	Nexus Engine - Win32 GUI测试程序
*
*	Author: Neil
*	Date:	Feb, 2009
*/

#pragma once
#include "ntest_case.h"

/**
 * 测试基本的static mesh的加载、渲染
*/
class basic_static_mesh_test :
	public nrender_test
{
public:
	basic_static_mesh_test(void);
	virtual ~basic_static_mesh_test(void);

	virtual void open(editor_camera_controller* cam);	

	nDECLARE_CLASS(basic_static_mesh_test)
};
