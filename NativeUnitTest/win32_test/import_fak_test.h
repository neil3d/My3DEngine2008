#pragma once
#include "ntest_case.h"

class import_fak_test :
	public nresource_test
{
public:
	import_fak_test(void);
	virtual ~import_fak_test(void);

	virtual void open(perspective_camera_controller* cam);

	nDECLARE_CLASS(import_fak_test)
};
