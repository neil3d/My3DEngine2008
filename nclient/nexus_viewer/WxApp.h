#pragma once

class WxApp : public wxApp
{
public:
	WxApp(void);
	virtual ~WxApp(void);

	virtual bool OnInit();
};
