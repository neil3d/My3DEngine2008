#pragma once
#include "ntest_case.h"

class tet_speed_tree_collision
	:public nphys_test
{
public:
	tet_speed_tree_collision(void);
	virtual ~tet_speed_tree_collision(void);

	virtual void open(perspective_camera_controller* cam);	

	void load_spt( const nstring& str_res, const vector3& pos, float scale);

private:
	int				m_st_index;

	nDECLARE_CLASS(tet_speed_tree_collision)

};
