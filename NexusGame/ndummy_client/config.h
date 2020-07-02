#pragma once

#include "nconfig.h"

enum EConfigIndex
{
	ECI_None					= 0,
	ECI_ServerIP,							// lan client的最大连接数
	ECI_ServerPort,							// world游戏世界端口
	ECI_ResPath,							// 资源目录
	ECI_Max,
};

using namespace nexus;

class config : public nconfig
{
public:
	config();
	~config();

	static config* instance();

	virtual bool init(const tstring& file_name, uint16 values_count);
	virtual void destroy();
};
