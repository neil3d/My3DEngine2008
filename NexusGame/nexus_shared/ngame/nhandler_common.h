#include "nclient_session.h"
#include "nprotocol_base.h"
#include "nnetwork_def.h"
#include "nplayer.h"
#include "nlog_mt.h"

namespace nexus {

#define MSG_STATIC_CAST(DATA_PTR, MSG_PTR, MSG_TYPE) \
	const tag##MSG_TYPE* DATA_PTR = static_cast<const tag##MSG_TYPE*>(MSG_PTR);

// 定长消息长度检查
#define CHECK_MSG_SIZE(MSG_PTR, MSG_TYPE) \
	if ((reinterpret_cast<const nmessage*>(reinterpret_cast<const char*>(msg_ptr) - NSMSG_OFFSET))->size != sizeof(tag##MSG_TYPE)) \
	{ NLOG_DEBUG(_T("msg size error!")); return; }

// 变长消息长度检查参考聊天消息的检验(nhandler_chat.cpp)

} // namespace nexus
