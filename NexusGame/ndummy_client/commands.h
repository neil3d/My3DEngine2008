#pragma once

#include "ntype_def.h"

/**
 *	客户端命令行处理函数
 */

class console_cmd;

class commands
{
public:
	commands();
	virtual ~commands();

	static commands* instance();

	void handle_chat(const console_cmd& cmd);
	void handle_cast_spell(const console_cmd& cmd);
};
