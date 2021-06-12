#pragma once
#include <shellscalingapi.h>

class CMyApp :
	public wxApp
{
public:
	CMyApp();
	~CMyApp();
	bool OnInit() wxOVERRIDE;
	void InitShcoreModule();

private:
	decltype(&SetProcessDpiAwareness) m_pfnSetProcessDpiAwareness;
private:
	HMODULE hShcoreModule;
};

