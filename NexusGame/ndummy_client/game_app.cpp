#include "StdAfx.h"
#include "msg_dispatch.h"
#include "game_app.h"
#include "game_map.h"
#include "local_player.h"
#include "OutputWnd.h"
#include "client.h"
#include "config.h"
#include "nutil.h"

#include "file/nstd_file_system.h"
#include "att_data/natt_info_data.h"

game_app::game_app(void)
{
	m_render_wnd = NULL;
}

game_app::~game_app(void)
{
}

game_app* game_app::instance()
{
	static game_app g_inst;
	return &g_inst;
}

void game_app::init()
{
	::timeBeginPeriod(1);

	//add by Leo:
	s_util.init();

	boost::shared_ptr<CCoreLogRedirect> log_out( new CCoreLogRedirect );
	nlog::instance()->add_log_output(log_out);

	config::instance()->init(_T("ndummy_client.cfg"), ECI_Max);

	_load_att_data();

	g_msg_dispatch.init();
	game_map::instance()->init();

	if (false == client::instance()->startup())
	{
		nLog_Info(_T("client startup error!!!"));
		return;
	}

	std::string ip = config::instance()->get_string(ECI_ServerIP);
	uint32 port = config::instance()->get_uint32(ECI_ServerPort);

	client::instance()->connect_to(ip, port);

	nLog_Info(_T("Nexus Dummy Client Init OK ..."));
}

void game_app::update(float delta_time)
{
	m_game_time += delta_time;
	g_msg_dispatch.update(uint32(delta_time * 1000));
	local_player::instance()->update(delta_time);
	game_map::instance()->update(delta_time);

	//-- 视口始终锁定本地玩家
	m_canvas.update_view(local_player::instance()->get_pos());
}

void game_app::render()
{
	if( m_render_wnd
		&& m_render_wnd->GetSafeHwnd() )
	{
		m_canvas.clear();
		
		game_map::instance()->draw(&m_canvas);		

		m_canvas.flip(m_render_wnd);
	}
}

void game_app::close()
{
	//add by Leo
	s_util.destroy();

	client::instance()->shutdown();
	::timeEndPeriod(1);
}

void game_app::bind_render_wnd(CWnd* wnd)
{
	m_render_wnd = wnd;
	
	if(wnd)
		m_canvas.init(wnd);
	else
		m_canvas.close();
}

void game_app::scale_view(int delta)
{
	int ts = m_canvas.get_scale() + delta;
	if(ts > 100)
		ts = 100;
	if(ts < 10)
		ts = 10;

	m_canvas.set_scale(ts);
}

bool game_app::_load_att_data()
{
	// 包名, 文件扩展
	const tstring INFO_PKG_NAME							= _T("attribute");
	const tstring SUFFIX_NAME							= _T(".att");

	// 文件名常量
	const tstring LEVELUP_INFO_FILE_NAME				= _T("nlevelup_info_dictionary") + SUFFIX_NAME;
	const tstring SPELL_INFO_FILE_NAME					= _T("nspell_info_dictionary") + SUFFIX_NAME;
	const tstring SPELL_EFFECT_INFO_FILE_NAME			= _T("nspell_effect_info_dictionary") + SUFFIX_NAME;
	const tstring SPELL_AURA_INFO_FILE_NAME				= _T("nspell_aura_info_dictionary") + SUFFIX_NAME;

	tstring path = config::instance()->get_tstring(ECI_ResPath);

	nstd_file_system fs;
	fs.init(path);

	nLog_Info(_T("loading %s!"), SPELL_INFO_FILE_NAME.c_str());

	if (!nspell_info_dictionary::instance()->serialize(&fs, INFO_PKG_NAME, SPELL_INFO_FILE_NAME, EFileRead))
	{
		nLog_Error(_T("load %s error!"), SPELL_INFO_FILE_NAME.c_str());
		return false;
	}

	nLog_Info(_T("loading %s!"), SPELL_EFFECT_INFO_FILE_NAME.c_str());

	if (!nspell_effect_info_dictionary::instance()->serialize(&fs, INFO_PKG_NAME, SPELL_EFFECT_INFO_FILE_NAME, EFileRead))
	{
		nLog_Error(_T("load %s error!"), SPELL_EFFECT_INFO_FILE_NAME.c_str());
		return false;
	}

	nLog_Info(_T("loading %s!"), SPELL_AURA_INFO_FILE_NAME.c_str());

	if (!nspell_aura_info_dictionary::instance()->serialize(&fs, INFO_PKG_NAME, SPELL_AURA_INFO_FILE_NAME, EFileRead))
	{
		nLog_Error(_T("load %s error!"), SPELL_AURA_INFO_FILE_NAME.c_str());
		return false;
	}

	nLog_Info(_T("load att data complete!"));
	return true;
}
