#include "stdafx.h"
#include "ControlWorkerThread.h"
#include "DriverToolFrame.h"
#include "ServiceControl.h"

class CDriverToolFrame;

CControlWorkerThread::CControlWorkerThread(CDriverToolFrame * pFrame)
{
	m_pFrame = pFrame;
}

CControlWorkerThread::~CControlWorkerThread()
{
}

wxThread::ExitCode CControlWorkerThread::Entry()
{
	wxString strNotify = "操作完成";
	TCHAR buff[1024];
	m_pFrame->DisableAllButton();
	do 
	{
		wxString strFile = m_pFrame->m_pEdtDriverPath->GetLabelText();
		CServiceControl sc;
		sc.SetPath(strFile.c_str().AsWCharBuf().data());

		switch (m_ulMessageID)
		{
		case INSTALL:
			if (sc.InstallService() != TRUE)
			{
				swprintf_s(buff, L"错误代码：%lu,%s", sc.GetErrorCode(), sc.GetErrorStr(sc.GetErrorCode()));
				strNotify = buff;
			}
			else
			{
				strNotify = wxT("服务安装成功");
			}
			break;

		case START:
			if (sc.RunService() != TRUE)
			{
				swprintf_s(buff, L"错误代码：%lu,%s", sc.GetErrorCode(), sc.GetErrorStr(sc.GetErrorCode()));
				strNotify = buff;
			}
			else
			{
				strNotify = wxT("服务启动成功");
			}
			break;

		case STOP:
			if (sc.StopService() != TRUE)
			{
				swprintf_s(buff, L"错误代码：%lu,%s", sc.GetErrorCode(), sc.GetErrorStr(sc.GetErrorCode()));
				strNotify = buff;
			}
			else
			{
				strNotify = wxT("服务停止成功");
			}
			break;

		case UNINSTALL:
			if (sc.UnInstallService() != TRUE)
			{
				swprintf_s(buff, L"错误代码：%lu,%s", sc.GetErrorCode(), sc.GetErrorStr(sc.GetErrorCode()));
				strNotify = buff;
			}
			else
			{
				strNotify = wxT("服务卸载成功");
			}
			break;

		default:
			strNotify = wxT("......我是谁？我在哪？我在做什么？");
		}

	} while (0);
	wxThreadEvent event(wxEVT_THREAD, THREAD_SERVER_CONTROL_COMPLETE);
	event.SetString(strNotify);
	wxQueueEvent(m_pFrame, event.Clone());

	m_pFrame->EnableAllButton();
	return NULL;
}

void CControlWorkerThread::OnExit()
{

}