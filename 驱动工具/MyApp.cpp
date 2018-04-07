#include "stdafx.h"
#include "MyApp.h"
#include "DriverToolFrame.h"

wxIMPLEMENT_APP(CMyApp);

CMyApp::CMyApp()
{
}


CMyApp::~CMyApp()
{
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

