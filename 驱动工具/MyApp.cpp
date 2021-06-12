#include "stdafx.h"
#include "MyApp.h"
#include "DriverToolFrame.h"

wxIMPLEMENT_APP(CMyApp);

CMyApp::CMyApp()
{
	hShcoreModule = nullptr;

	InitShcoreModule();
}


CMyApp::~CMyApp()
{
	if (hShcoreModule != nullptr)
	{
		FreeLibrary(hShcoreModule);
		hShcoreModule = nullptr;
	}
}

void CMyApp::InitShcoreModule()
{
	do
	{
		hShcoreModule = LoadLibraryW(L"Shcore.dll");
		if (hShcoreModule == nullptr)
		{
			break;
		}

		m_pfnSetProcessDpiAwareness = (decltype(&SetProcessDpiAwareness))GetProcAddress(hShcoreModule, "SetProcessDpiAwareness");
		if (m_pfnSetProcessDpiAwareness == nullptr)
		{
			break;
		}
		m_pfnSetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
	} while (false);
}

bool CMyApp::OnInit()
{
	bool bRet = true;

	do 
	{
		if (!wxApp::OnInit())
		{
			bRet = false;
			break;
		}

		CDriverToolFrame *pFrame = new CDriverToolFrame;
		pFrame->Show(true);

	} while (0);

	return bRet;
}

