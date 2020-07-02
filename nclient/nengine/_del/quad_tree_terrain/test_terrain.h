/**
*	Nexus Engine - Win32 GUI测试程序
*
*	Author: Neil
*	Date:	Feb, 2009
*/

#pragma once
#include "ncore.h"
#include "ntest_case.h"

/**
 * 测试地形的加载，渲染
*/
class test_terrain :
	public nrender_test
{
public:
	test_terrain(void);
	virtual ~test_terrain(void);

	virtual void open(editor_camera_controller* cam);	

private:		
	nDECLARE_CLASS(test_terrain)
};
