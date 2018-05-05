#include "stdafx.h"
#include "DriverToolFrame.h"
#include "ServiceControl.h"
#include "ControlWorkerThread.h"
#include "resource.h"
#include <wx/collpane.h>
#include <wx/stattext.h>
#include <map>
#include <Windows.h>
#include <cassert>
#include <wx/fontutil.h>
#include "IoctlEdtControl.h"

wxDEFINE_EVENT(wxEVT_MY_CUSTOM_COMMAND, wxCommandEvent);


wxBEGIN_EVENT_TABLE(CDriverToolFrame, wxFrame)
	EVT_BUTTON(ID_BTN_SELECTPATH, CDriverToolFrame::OnSelectFile)
	EVT_BUTTON(ID_BTN_INSTALL, CDriverToolFrame::OnInstall)
	EVT_BUTTON(ID_BTN_START, CDriverToolFrame::OnStart)
	EVT_BUTTON(ID_BTN_STOP, CDriverToolFrame::OnStop)
	EVT_BUTTON(ID_BTN_UNINSTALL, CDriverToolFrame::OnUnInstall)
	EVT_THREAD(THREAD_SERVER_CONTROL_COMPLETE, CDriverToolFrame::OnServiceControlComplete)
	EVT_CHECKBOX(ID_CHK_WINDOWTOP, CDriverToolFrame::OnWindowTop)
	EVT_DROP_FILES(CDriverToolFrame::OnDropFile)
	EVT_COLLAPSIBLEPANE_CHANGED(ID_COLLAPSIBLEPANE, OnCollapsiblePaneExpand)

	//ExtPandFirst
	EVT_RADIOBOX(ID_RADIOBOX_PAGE1, OnStartChange)
	EVT_CHECKBOX(ID_RADIOBEGIN + 0, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 1, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 2, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 3, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 4, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 5, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 6, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 7, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 8, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 9, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 10, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 11, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 12, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 13, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 14, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 15, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 16, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 17, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 18, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 19, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 20, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 21, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 22, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 23, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 24, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 25, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 26, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 27, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 28, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 29, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 30, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 31, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 32, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 33, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 34, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 35, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 36, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 37, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 38, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 39, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 40, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 41, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 42, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 43, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 44, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 45, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 46, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 47, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 48, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 49, OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 50, OnFilterDriverNotify)

	//Second Panel
	EVT_TEXT(ID_EDTSHOWIOCTL, OnIoctlCodeChange)
	EVT_TEXT(ID_EDTSHOWMNEMONIC, OnIoctlMnemonicChange)
	EVT_SPIN(ID_SPIL_DEVICETYPE, OnSpinClick)
	EVT_SPIN(ID_SPIL_FUNCTION, OnSpinClick)
	EVT_SPIN(ID_SPIL_METHOD, OnSpinClick)
	EVT_SPIN(ID_SPIL_ACCESS, OnSpinClick)
wxEND_EVENT_TABLE()

std::map<wxString, wxString> g_GUIDMap;

CDriverToolFrame::CDriverToolFrame() :wxFrame(NULL, wxID_ANY, wxT("驱动工具"), wxDefaultPosition, wxDefaultSize, wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX | wxCAPTION | wxCLIP_CHILDREN)
{
	InitWDMFilterData();
	wxFont font;
	font.Create(13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("微软雅黑"));
	SetIcon(wxICON(icon));
	CreateStatusBar();
	SetStatusText(wxT("欢迎使用"));

	this->SetBackgroundColour(wxColor(255, 255, 255));
	this->DragAcceptFiles(true);

	m_pMainBoxSizer = new wxBoxSizer(wxVERTICAL);

	m_pStaticBoxTopSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, wxT("文件"));

	m_pStaticPath = new wxStaticText(this, wxID_ANY, wxT("文件路径:"));
	m_pStaticPath->SetFont(font);
	m_pStaticBoxTopSizer->Add(m_pStaticPath, 0, wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL | wxALL, 5);

	m_pEdtDriverPath = new wxTextCtrl(this, ID_EDIT_FILEPATH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
	m_pEdtDriverPath->SetFont(font);
	m_pStaticBoxTopSizer->Add(m_pEdtDriverPath, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_pBtnSelectPath = new wxButton(this, ID_BTN_SELECTPATH, wxT("..."), wxDefaultPosition, wxSize(50,30));
	m_pStaticBoxTopSizer->Add(m_pBtnSelectPath, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 5);

	m_pChkBoxWindowTop = new wxCheckBox(this, ID_CHK_WINDOWTOP, wxT("窗口置顶"));
	m_pChkBoxWindowTop->SetFont(font);
	m_pStaticBoxTopSizer->Add(m_pChkBoxWindowTop, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 5);

	m_pMiddleStaticBoxSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, wxT("控制"));
	m_pBtnInstall = new wxButton(this, ID_BTN_INSTALL, wxT("安装"));
	m_pBtnInstall->SetFont(font);
	m_pMiddleStaticBoxSizer->Add(m_pBtnInstall, 1, wxALIGN_CENTRE_VERTICAL | wxALL, 5);
	
	m_pBtnStart = new wxButton(this, ID_BTN_START, wxT("启动"));
	m_pBtnStart->SetFont(font);
	m_pMiddleStaticBoxSizer->Add(m_pBtnStart, 1, wxALIGN_CENTRE_VERTICAL | wxALL, 5);

	m_pBtnStop = new wxButton(this, ID_BTN_STOP, wxT("停止"));
	m_pBtnStop->SetFont(font);
	m_pMiddleStaticBoxSizer->Add(m_pBtnStop, 1, wxALIGN_CENTRE_VERTICAL | wxALL, 5);

	m_pBtnUninstall = new wxButton(this, ID_BTN_UNINSTALL, wxT("卸载"));
	m_pBtnUninstall->SetFont(font);
	m_pMiddleStaticBoxSizer->Add(m_pBtnUninstall, 1, wxALIGN_CENTRE_VERTICAL | wxALL, 5);

	m_pBottomBoxSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, wxT("状态"));
	m_pStaticStatus = new wxStaticText(this, wxID_ANY, wxT("状态:"));
	m_pStaticStatus->SetFont(font);
	m_pBottomBoxSizer->Add(m_pStaticStatus, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_pEdtShow = new wxTextCtrl(this, ID_EDT_SHOW, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
	m_pEdtShow->SetFont(font);
	m_pBottomBoxSizer->Add(m_pEdtShow, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_pStaticName = new wxStaticText(this, wxID_ANY, wxT("By.Hell"));
	m_pStaticName->SetFont(font);
	m_pBottomBoxSizer->Add(m_pStaticName, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_pOtherBoxSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, wxT("进度条"));

	m_pGauge = new wxGauge(this, ID_GAUGE, 100);
	m_pOtherBoxSizer->Add(m_pGauge, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	//Extend Panel
	

	m_pMainBoxSizer->Add(m_pStaticBoxTopSizer, 1, wxEXPAND, 10);
	m_pMainBoxSizer->Add(m_pMiddleStaticBoxSizer, 1, wxEXPAND, 10);
	m_pMainBoxSizer->Add(m_pBottomBoxSizer, 1, wxEXPAND, 10);
	m_pMainBoxSizer->Add(m_pOtherBoxSizer, 1, wxEXPAND, 10);

	InitCollapsiblePane();

	this->SetSizer(m_pMainBoxSizer);
	m_pMainBoxSizer->SetMinSize(wxSize(600, 300));
	m_pMainBoxSizer->SetSizeHints(this);
}

CDriverToolFrame::~CDriverToolFrame()
{
}

void CDriverToolFrame::OnSelectFile(wxCommandEvent & event)
{
	wxFileDialog fileDialog(
		this,
		wxT("选择文件"),
		wxEmptyString,
		wxEmptyString,
		wxT("sys files (*.sys)|*.sys"),
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	do 
	{
		if (fileDialog.ShowModal() == wxID_CANCEL)
		{
			break;
		}

		wxString szPath = fileDialog.GetPath();
		m_pEdtDriverPath->SetLabelText(szPath);

		ClearCheckBox();
		NotifyExtFrame();
	} while (0);
}

void CDriverToolFrame::OnInstall(wxCommandEvent & event)
{
	wxString szFile = m_pEdtDriverPath->GetValue();
	do 
	{
		if (szFile == wxT(""))
		{
			m_pEdtShow->SetLabelText(wxT("请选择文件"));
			break;
		}

		CControlWorkerThread *pThread = new CControlWorkerThread(this);
		if (pThread->Create() != wxTHREAD_NO_ERROR)
		{
			m_pEdtShow->SetLabelText(wxT("线程创建失败"));
			break;
		}

		pThread->m_ulMessageID = INSTALL;

		if (pThread->Run() != wxTHREAD_NO_ERROR)
		{
			m_pEdtShow->SetLabelText(wxT("线程启动失败"));
			break;
		}
	} while (0);
	
}

void CDriverToolFrame::OnStart(wxCommandEvent & event)
{
	wxString szFile = m_pEdtDriverPath->GetLabelText();
	do
	{
		if (szFile == "")
		{
			m_pEdtShow->SetLabelText(wxT("请选择文件"));
			break;
		}

		CServiceControl sc;
		sc.SetPath(szFile.c_str().AsWCharBuf().data());
		if (sc.ImageCheck() == FALSE)
		{
			if (wxMessageBox(wxT("目标文件格式和当前系统不符，是否继续加载？"), wxT("格式检查"), wxYES_NO, this) == wxNO)
			{
				break;
			}
		}

		CControlWorkerThread *pThread = new CControlWorkerThread(this);
		if (pThread->Create() != wxTHREAD_NO_ERROR)
		{
			m_pEdtShow->SetLabelText(wxT("线程创建失败"));
			break;
		}

		pThread->m_ulMessageID = START;

		if (pThread->Run() != wxTHREAD_NO_ERROR)
		{
			SetStatusText(wxT("线程启动失败"));
			m_pEdtShow->SetLabelText(wxT("线程启动失败"));
			break;
		}
	} while (0);
}

void CDriverToolFrame::OnStop(wxCommandEvent & event)
{
	wxString szFile = m_pEdtDriverPath->GetLabelText();
	do
	{
		if (szFile == "")
		{
			m_pEdtShow->SetLabelText(wxT("请选择文件"));
			break;
		}

		CControlWorkerThread *pThread = new CControlWorkerThread(this);
		if (pThread->Create() != wxTHREAD_NO_ERROR)
		{
			m_pEdtShow->SetLabelText(wxT("线程创建失败"));
			break;
		}

		pThread->m_ulMessageID = STOP;

		if (pThread->Run() != wxTHREAD_NO_ERROR)
		{
			m_pEdtShow->SetLabelText(wxT("线程启动失败"));
			break;
		}
	} while (0);
}

void CDriverToolFrame::OnUnInstall(wxCommandEvent & event)
{
	wxString szFile = m_pEdtDriverPath->GetLabelText();
	do
	{
		if (szFile == "")
		{
			m_pEdtShow->SetLabelText(wxT("请选择文件"));
			break;
		}

		CControlWorkerThread *pThread = new CControlWorkerThread(this);
		if (pThread->Create() != wxTHREAD_NO_ERROR)
		{
			m_pEdtShow->SetLabelText(wxT("线程创建失败"));
			break;
		}

		pThread->m_ulMessageID = UNINSTALL;

		if (pThread->Run() != wxTHREAD_NO_ERROR)
		{
			m_pEdtShow->SetLabelText(wxT("线程启动失败"));
			break;
		}
	} while (0);
}

void CDriverToolFrame::OnWindowTop(wxCommandEvent & event)
{
	if (m_pChkBoxWindowTop->IsChecked() == true)
	{
		SetWindowPos(this->GetHandle(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
	else
	{
		SetWindowPos(this->GetHandle(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
}

void CDriverToolFrame::OnWindowMove(wxMoveEvent & event)
{

}

void CDriverToolFrame::OnStartChange(wxCommandEvent & event)
{
	int nSelect = m_pRadioBoxStartOption->GetSelection();
	wxString keyPath = L"SYSTEM\\CurrentControlSet\\Services\\" + m_szServiceName;

	HKEY hMainKey = NULL;
	ULONG ulRet = 0;
	do
	{
		if (m_szServiceName == wxT(""))
		{
			SetStatusText(wxT("请选择一个文件"));
			m_pRadioBoxStartOption->SetSelection(3);
			break;
		}

		ulRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, keyPath.c_str(), 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hMainKey);
		if (ulRet != ERROR_SUCCESS)
		{
			SetStatusText(wxT("注册表访问失败，请确定服务已经安装或该程序有足够高权限"));
			break;
		}

		ulRet = sizeof(DWORD);
		ulRet = RegSetValueEx(hMainKey, TEXT("Start"), 0, REG_DWORD, (const PBYTE)&nSelect, sizeof(DWORD));
		if (ulRet != ERROR_SUCCESS)
		{
			break;
		}
	} while (0);

	if (hMainKey != NULL)
	{
		RegCloseKey(hMainKey);
	}
}

void CDriverToolFrame::OnFilterDriverNotify(wxCommandEvent & event)
{
	wxCheckBox *p = (wxCheckBox*)event.GetEventObject();
	wxString szFilterDevice = p->GetLabelText();
	wxString szGUID = g_GUIDMap[szFilterDevice];
	wxString keyPath;
	HKEY hMainKey = NULL;
	TCHAR szDriverList[1024] = { 0 };
	DWORD dwRetSize;

	ULONG ulRet = -1;
	do
	{
		if (szGUID == wxT(""))
		{
			SetStatusText(wxT("没找到对应GUID？ emmm，这有Bug"));
			break;
		}

		if (m_szServiceName == wxT(""))
		{
			SetStatusText(wxT("请选择一个文件"));
			break;
		}

		keyPath = TEXT("SYSTEM\\CurrentControlSet\\Control\\Class\\") + szGUID;
		ulRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, keyPath.c_str(), 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hMainKey);
		if (ulRet != ERROR_SUCCESS)
		{
			SetStatusText(wxT("RegOpenKeyEx 失败"));
			break;
		}

		dwRetSize = 2048;
		//ulRet = RegGetValue(hMainKey, NULL, TEXT("UpperFilters"), RRF_RT_REG_MULTI_SZ | RRF_RT_REG_SZ, NULL, szDriverList, &dwRetSize);
		ulRet = RegQueryValueEx(hMainKey, TEXT("UpperFilters"), 0, NULL, (const PBYTE)szDriverList, &dwRetSize);
		if (ulRet != ERROR_SUCCESS)
		{
			if (ulRet != ERROR_FILE_NOT_FOUND)
			{
				swprintf_s(szDriverList, L"RegGetValue 失败:%lu", ulRet);
				SetStatusText(szDriverList);
				break;
			}
		}


		if (p->GetValue() == true)
		{
			//说明之前没有，是要添加
			wxArrayString serviceArray;
			wxString szItem;
			bool bFind = false;
			TCHAR *p = szDriverList;
			
			serviceArray.push_back(m_szServiceName);
			while (*p != 0)
			{
				if (!_tcsicmp(m_szServiceName.c_str(), p))
				{
					bFind = true;
					break;
				}

				serviceArray.push_back(p);
				p += _tcslen(p) + 1;
			}

			if (bFind == true)
			{
				//找到了？再见
				break;
			}

			memset(szDriverList, 0, 1024 * sizeof(TCHAR));
			p = szDriverList;
			auto iter = serviceArray.begin();
			size_t length = 0;
			for (; iter != serviceArray.end(); ++iter)
			{
				_tcscpy_s(p, 1024 - length, iter->c_str());
				length += iter->Length();
				p += iter->Length();

				p += 1;
				length += 1;
			}

			//_tcscpy_s(p, 1024 - (p - szDriverList), m_szServiceName.c_str());
			//p += _tcslen(p) + 1;
			//*p = 0;

			ulRet = RegSetValueEx(hMainKey, TEXT("UpperFilters"), 0, REG_MULTI_SZ, (CONST BYTE*)szDriverList, length * sizeof(TCHAR));
			if (ulRet != ERROR_SUCCESS)
			{
				break;
			}
		}
		else
		{
			//去掉
			bool bFind = false;
			TCHAR *p = szDriverList;
			wxArrayString serviceArray;
			wxString szItem;
			while (*p != 0)
			{
				szItem = p;
				serviceArray.Add(szItem);
				p += szItem.Length() + 1;
			}

			auto iter = serviceArray.begin();
			for (; iter != serviceArray.end();)
			{
				if (*iter == m_szServiceName)
				{
					iter = serviceArray.erase(iter);
				}
				else
				{
					++iter;
				}
			}

			memset(szDriverList, 0, 1024 * sizeof(TCHAR));
			p = szDriverList;
			iter = serviceArray.begin();
			size_t length = 0;
			for (; iter != serviceArray.end(); ++iter)
			{
				_tcscpy_s(p, 1024 - length, iter->c_str());
				length += iter->Length();
				p += iter->Length();

				p += 1;
				length += 1;
			}

			ulRet = RegSetValueEx(hMainKey, TEXT("UpperFilters"), 0, REG_MULTI_SZ, (CONST BYTE*)szDriverList, length * sizeof(TCHAR));
			if (ulRet != ERROR_SUCCESS)
			{
				break;
			}
			ulRet = 0;
		}


	} while (0);

	if (hMainKey != NULL)
	{
		RegCloseKey(hMainKey);
	}

	if (ulRet == ERROR_SUCCESS)
	{
		SetStatusText(wxT("操作完成"));
	}
	else
	{
		p->SetValue(!p->GetValue());
	}
}

void CDriverToolFrame::DisableAllButton()
{
	if (m_pBtnInstall != NULL)
	{
		m_pBtnInstall->Enable(false);
	}
	
	if (m_pBtnStart != NULL)
	{
		m_pBtnStart->Enable(false);
	}

	if (m_pBtnStop != NULL)
	{
		m_pBtnStop->Enable(false);
	}

	if (m_pBtnUninstall != NULL)
	{
		m_pBtnUninstall->Enable(false);
	}

	if (m_pGauge != NULL)
	{
		m_pGauge->Pulse();
	}
}

void CDriverToolFrame::EnableAllButton()
{
	if (m_pBtnInstall != NULL)
	{
		m_pBtnInstall->Enable(true);
	}

	if (m_pBtnStart != NULL)
	{
		m_pBtnStart->Enable(true);
	}

	if (m_pBtnStop != NULL)
	{
		m_pBtnStop->Enable(true);
	}

	if (m_pBtnUninstall != NULL)
	{
		m_pBtnUninstall->Enable(true);
	}

	if (m_pGauge != NULL)
	{
		m_pGauge->SetValue(0);
	}
}

void CDriverToolFrame::OnServiceControlComplete(wxThreadEvent & event)
{
	wxString strControlRet = event.GetString();
	m_pEdtShow->SetLabelText(strControlRet);
	SetStatusText(strControlRet);
}

void CDriverToolFrame::OnDropFile(wxDropFilesEvent & event)
{
	if (event.GetNumberOfFiles() > 0)
	{
		m_pEdtDriverPath->SetLabelText(event.GetFiles()[0]);

		ClearCheckBox();
		NotifyExtFrame();
	}
}

void CDriverToolFrame::OnCollapsiblePaneExpand(wxCollapsiblePaneEvent & event)
{
	do 
	{
		if (m_pExtendPanel->IsCollapsed() == true)
		{
			SetStatusText(wxT("欢迎使用"));
			break;
		}

		NotifyExtFrame();
	} while (0);
}

void CDriverToolFrame::OnIoctlCodeChange(wxCommandEvent & event)
{
	static wxString strLastCode = wxT("");
	wxString strCode;
	strCode = m_pEdtShowIoctlCode->GetValue();

	if (strLastCode == strCode)
	{
		return;
	}
	strLastCode = strCode;

	auto pCheckCode = [&]()->bool
	{
		bool bRet = false;
		do 
		{
			if (strCode.length() < 8)
			{
				break;
			}

			if (strCode.length() > 10)
			{
				break;
			}

			wxString strNewCode;
			size_t pos = strCode.rfind(wxT('x'));
			if (pos != wxString::npos)
			{
				strNewCode = wxString(strCode, pos + 1, strCode.Length() - pos - 1);
			}
			else
			{
				strNewCode = strCode;
			}

			if (strNewCode.length() != 8)
			{
				break;
			}

			bool bCodeCheck = true;
			for (auto a : strNewCode)
			{
				if (a >= wxT('0') && a <= wxT('9'))
				{
					continue;
				}
				else if (a >= wxT('a') && a <= wxT('f'))
				{
					continue;
				}
				else if (a >= wxT('A') && a <= wxT('F'))
				{
					continue;
				}
				else
				{
					bCodeCheck = false;
					break;
				}
			}

			if (bCodeCheck == false)
			{
				break;
			}
			
			bRet = true;
		} while (0);
		
		return bRet;
	};

	IOCTL_INFO ioctlInfo = { 0 };
	do 
	{
		if (pCheckCode() == false)
		{
			break;
		}

		TCHAR *p = nullptr;
		ioctlInfo.ulData = _tcstol(strCode.c_str(), &p, 16);
	} while (0);

	UpdateIoctlInfo(ioctlInfo);
}

void CDriverToolFrame::OnIoctlMnemonicChange(wxCommandEvent & event)
{
	static wxString strLastData = wxT("");
	wxString strtemp;
	strtemp = m_pEdtMnemonic->GetValue();
	if (strLastData != strtemp)
	{
		strLastData = strtemp;
		m_pIoctlControl->MnemonicSet(strtemp);
	}
	
}

void CDriverToolFrame::OnSpinClick(wxSpinEvent & event)
{
	wxObject* pObject = event.GetEventObject();

	do 
	{
		if (pObject == nullptr)
		{
			break;
		}

		IOCTL_INFO ioctlInfo = { 0 };
		m_pIoctlControl->GetIoctlInfo(ioctlInfo);

		if (pObject == m_pSpinDeviceType)
		{
			ioctlInfo.Info.DeviceType = m_pSpinDeviceType->GetValue();
		}
		else if (pObject == m_pSpinFunction)
		{
			ioctlInfo.Info.Function = m_pSpinFunction->GetValue();
		}
		else if (pObject == m_pSpinMethod)
		{
			ioctlInfo.Info.Method = m_pSpinMethod->GetValue();
		}
		else if (pObject == m_pSpinAccess)
		{
			ioctlInfo.Info.Access = m_pSpinAccess->GetValue();
		}
		else
		{
			break;
		}

		m_pIoctlControl->UpdateIoctlInfo(ioctlInfo);
	} while (0);
}

void CDriverToolFrame::InitCollapsiblePane()
{
	m_pCollapsibleSizer = new wxBoxSizer(wxVERTICAL);
	
	wxWindow *pMainPanel;
	m_pExtendPanel = new wxCollapsiblePane(this, ID_COLLAPSIBLEPANE, wxT("更多"));
	m_pExtendPanel->SetForegroundColour(wxColor(255, 255, 255));

	pMainPanel = m_pExtendPanel->GetPane();
	m_pCollapsibleSizer->Add(m_pExtendPanel, 0, wxGROW | wxALL, 5);

	m_pExtenPanelMainBoxSizer = new wxBoxSizer(wxVERTICAL);
	m_pNotebook = new wxNotebook(pMainPanel, ID_NOTEBOOK);

	auto InitlizeFirstPanel = [&] 
	{
		//Panel 1
		m_pExtBoxSize1 = new wxBoxSizer(wxVERTICAL);
		wxArrayString strRadioBoxs;
		strRadioBoxs.Add(wxT("BOOT_START"));
		strRadioBoxs.Add(wxT("SYSTEM_START"));
		strRadioBoxs.Add(wxT("AUTO_START"));
		strRadioBoxs.Add(wxT("DEMAND_START"));
		strRadioBoxs.Add(wxT("DISABLED"));

		m_pFirstPanel = new wxPanel(m_pNotebook);
		m_pNotebook->AddPage(m_pFirstPanel, wxT("详细设置"), true);

		m_pRadioBoxStartOption = new wxRadioBox(m_pFirstPanel, ID_RADIOBOX_PAGE1, wxT("启动方式"), wxDefaultPosition, wxDefaultSize, strRadioBoxs, 0, wxRA_SPECIFY_COLS);
		m_pRadioBoxStartOption->SetSelection(3);
		m_pExtBoxSize1->Add(m_pRadioBoxStartOption, 0, wxALL | wxEXPAND, 5);

		m_pBottomStaticBoxSizer = new wxStaticBoxSizer(wxVERTICAL, m_pFirstPanel, wxT("附加设备(非WDM驱动勿选)"));
		m_pCheckBoxSizer = new wxGridSizer(0, 5, 0, 0);
		m_pBottomStaticBoxSizer->Add(m_pCheckBoxSizer, 1, wxALL | wxEXPAND, 5);
		m_pExtBoxSize1->Add(m_pBottomStaticBoxSizer, 1, wxALL | wxEXPAND, 5);

		m_ppCheckBoxArray = new wxCheckBox*[g_GUIDMap.size()];
		auto iter = g_GUIDMap.cbegin();
		for (int index = 0; iter != g_GUIDMap.cend(); ++iter, ++index)
		{
			m_ppCheckBoxArray[index] = new wxCheckBox(m_pFirstPanel, ID_RADIOBEGIN + index, iter->first);
			m_pCheckBoxSizer->Add(m_ppCheckBoxArray[index]);
		}

		m_pFirstPanel->SetSizer(m_pExtBoxSize1);
	};
	InitlizeFirstPanel();
	
	//Panel 2
	auto InitlizeSecondPanel = [&]
	{
		m_pExtSecondPanel = new wxPanel(m_pNotebook);
		m_pNotebook->AddPage(m_pExtSecondPanel, wxT("IOCTL"));

		m_pExtBoxSize2 = new wxBoxSizer(wxVERTICAL);
		m_pIoctlInfoSizer = new wxStaticBoxSizer(wxVERTICAL, m_pExtSecondPanel, wxT("IOCTL信息"));

		//
		wxSizer* pSizer = new wxBoxSizer(wxHORIZONTAL);
		m_vecSizerPointArray.push_back(pSizer);

		m_pStaticIoctlNumber = new wxStaticText(m_pExtSecondPanel, wxID_ANY, wxT("Ioctl Code:"));
		pSizer->Add(m_pStaticIoctlNumber, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
		
		m_pEdtShowIoctlCode = new wxTextCtrl(m_pExtSecondPanel, ID_EDTSHOWIOCTL);
		m_pEdtShowIoctlCode->SetMaxLength(8);
		pSizer->Add(m_pEdtShowIoctlCode, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

		m_pStaticNumberOfIoctl = new wxStaticText(m_pExtSecondPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
		pSizer->Add(m_pStaticNumberOfIoctl, 1, wxALIGN_CENTER_VERTICAL | wxALL, 2);

		m_pIoctlInfoSizer->Add(pSizer, 0, wxGROW | wxALL, 0);

		//
		pSizer = new wxBoxSizer(wxHORIZONTAL);
		m_vecSizerPointArray.push_back(pSizer);

		m_pStaticMnemonic = new wxStaticText(m_pExtSecondPanel, wxID_ANY, wxT("Mnemonic:"));

		pSizer->Add(m_pStaticMnemonic, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 2);

		m_pEdtMnemonic = new wxTextCtrl(m_pExtSecondPanel, ID_EDTSHOWMNEMONIC);
		pSizer->Add(m_pEdtMnemonic, 1, wxALIGN_CENTRE_VERTICAL | wxALL, 2);

		m_pIoctlInfoSizer->Add(pSizer, 0, wxGROW | wxALL, 0);

		m_pExtBoxSize2->Add(m_pIoctlInfoSizer, 0, wxGROW | wxALL, 5);
		
		m_pIoctlSetailsSizer = new wxStaticBoxSizer(wxVERTICAL, m_pExtSecondPanel, wxEmptyString);
		m_pFlexGridSizer = new wxFlexGridSizer(3);
		m_pFlexGridSizer->AddGrowableCol(1);

		//
		m_pStaticDeviceType = new wxStaticText(m_pExtSecondPanel, wxID_ANY, wxT("Device Type:"));
		m_pFlexGridSizer->Add(m_pStaticDeviceType, wxSizerFlags().CenterVertical());

		m_pEdtDeviceType = new wxTextCtrl(m_pExtSecondPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
		m_pFlexGridSizer->Add(m_pEdtDeviceType, wxSizerFlags(1).Expand().CenterVertical().Border());

		m_pSpinDeviceType = new wxSpinButton(m_pExtSecondPanel, ID_SPIL_FUNCTION);
		m_pSpinDeviceType->SetRange(0, 0xFFFF);
		m_pFlexGridSizer->Add(m_pSpinDeviceType, wxSizerFlags().CenterVertical());

		//
		m_vecSizerPointArray.push_back(pSizer);

		m_pStaticFunction = new wxStaticText(m_pExtSecondPanel, wxID_ANY, wxT("Function:"));
		m_pFlexGridSizer->Add(m_pStaticFunction, wxSizerFlags(0).CenterVertical());

		m_pEdtFunction = new wxTextCtrl(m_pExtSecondPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
		m_pFlexGridSizer->Add(m_pEdtFunction, wxSizerFlags(1).Expand().CenterVertical().Border());

		m_pSpinFunction = new wxSpinButton(m_pExtSecondPanel, ID_SPIL_FUNCTION);
		m_pSpinFunction->SetRange(0, 0xFFF);
		m_pFlexGridSizer->Add(m_pSpinFunction, wxSizerFlags(0).CenterVertical());
		
		

		m_vecSizerPointArray.push_back(pSizer);

		m_pStaticMethod = new wxStaticText(m_pExtSecondPanel, wxID_ANY, wxT("Method:"));
		m_pFlexGridSizer->Add(m_pStaticMethod, wxSizerFlags().CenterVertical());

		m_pEdtMethod = new wxTextCtrl(m_pExtSecondPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
		m_pFlexGridSizer->Add(m_pEdtMethod, wxSizerFlags(1).Expand().CenterVertical().Border());

		m_pSpinMethod = new wxSpinButton(m_pExtSecondPanel, ID_SPIL_METHOD);
		m_pSpinMethod->SetRange(0, 3);
		m_pFlexGridSizer->Add(m_pSpinMethod, wxSizerFlags().CenterVertical());

		//
		m_pStaticAccess = new wxStaticText(m_pExtSecondPanel, wxID_ANY, wxT("Access:"));
		m_pFlexGridSizer->Add(m_pStaticAccess, wxSizerFlags().CenterVertical());

		m_pEdtAccess = new wxTextCtrl(m_pExtSecondPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
		m_pFlexGridSizer->Add(m_pEdtAccess, wxSizerFlags(1).Expand().CenterVertical().Border());

		m_pSpinAccess = new wxSpinButton(m_pExtSecondPanel, ID_SPIL_FUNCTION);
		m_pSpinAccess->SetRange(0, 3);
		m_pFlexGridSizer->Add(m_pSpinAccess, wxSizerFlags().CenterVertical());

		m_pIoctlSetailsSizer->Add(m_pFlexGridSizer, 0, wxGROW | wxALL, 0);
		m_pExtBoxSize2->Add(m_pIoctlSetailsSizer, 0, wxGROW | wxALL, 5);

		//Layout
		m_pIoctlLayout = new wxStaticBoxSizer(wxVERTICAL, m_pExtSecondPanel, wxT("I/O Control Code Layout"));

		wxFont ioctlFont(9, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New"));

		pSizer = new wxBoxSizer(wxVERTICAL);
		m_vecSizerPointArray.push_back(pSizer);
		m_pEdtIoctlLayout = new wxTextCtrl(m_pExtSecondPanel, wxID_ANY, wxT("00000000000000000000000000000000"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTER);
		m_pEdtIoctlLayout->SetFont(ioctlFont);
		pSizer->Add(m_pEdtIoctlLayout, 0, wxGROW | wxALL, 0);

		wxBitmap ioctlLayoutBitmap(wxBITMAP(IOCTL_LAYOUT));
		m_pShowIoctlLayout = new wxStaticBitmap(m_pExtSecondPanel, wxID_ANY, ioctlLayoutBitmap);
		pSizer->Add(m_pShowIoctlLayout, 1, wxGROW | wxALL, 0);

		//初始化Ioctl助记符
		m_pIoctlControl = new CIoctlEdtControl(m_pEdtShowIoctlCode, m_pEdtMnemonic, m_pEdtDeviceType, m_pEdtFunction, m_pEdtMethod, m_pEdtAccess, m_pEdtIoctlLayout);
		m_pEdtShowIoctlCode->SetLabelText(wxT("000B0000"));

		wxString strTemp;
		strTemp = wxString::Format(wxT("Recognizes %lu mnemonics"), m_pIoctlControl->GetMnemonicNumber());
		m_pStaticNumberOfIoctl->SetLabelText(strTemp);

		m_pIoctlLayout->Add(pSizer, 1, wxGROW | wxALL, 0);
		m_pExtBoxSize2->Add(m_pIoctlLayout, 0, wxGROW | wxALL, 0);
		
		m_pExtSecondPanel->SetSizer(m_pExtBoxSize2);
	};
	InitlizeSecondPanel();
	

	///////////////////////////
	
	m_pExtenPanelMainBoxSizer->Add(m_pNotebook, 1, wxEXPAND | wxALL, 5);
	pMainPanel->SetSizer(m_pExtenPanelMainBoxSizer);
	m_pExtenPanelMainBoxSizer->SetSizeHints(pMainPanel);

	m_pMainBoxSizer->Add(m_pCollapsibleSizer);
}

void CDriverToolFrame::InitWDMFilterData()
{
	HKEY hRegKey = NULL;
	HKEY hSubRegKey = NULL;
	LONG lRet = 0;
	DWORD dwIndex = 0;
	DWORD nKeyNameBuffSize = 32767;
	wchar_t *pKeyName = NULL;
	wchar_t *pClassName = NULL;

	do 
	{
		pKeyName = (wchar_t*)calloc(32767, sizeof(wchar_t));
		if (pKeyName == NULL)
		{
			break;
		}
		memset(pKeyName, 0, 32767 * sizeof(wchar_t));

		pClassName = (wchar_t*)calloc(32767, sizeof(wchar_t));
		if (pClassName == NULL)
		{
			break;
		}
		memset(pClassName, 0, 32767 * sizeof(wchar_t));

		lRet = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\Class", 0, KEY_READ | KEY_WOW64_64KEY, &hRegKey);
		if (lRet != ERROR_SUCCESS)
		{
			break;
		}

		dwIndex = 0;
		nKeyNameBuffSize = 32767;
		while (RegEnumKeyExW(hRegKey, dwIndex, pKeyName, &nKeyNameBuffSize, 0, 0, 0, 0) == ERROR_SUCCESS)
		{
			do 
			{
				lRet = RegOpenKeyExW(hRegKey, pKeyName, 0, KEY_READ | KEY_WOW64_64KEY, &hSubRegKey);
				if (lRet != ERROR_SUCCESS)
				{
					break;
				}

				nKeyNameBuffSize = 32767;
				lRet = RegQueryValueExW(hSubRegKey, L"Class", 0, 0, (LPBYTE)pClassName, &nKeyNameBuffSize);
				if (lRet != ERROR_SUCCESS)
				{
					break;
				}

				g_GUIDMap.insert(std::pair<wxString, wxString>(pClassName, pKeyName));
			} while (0);

			if (hSubRegKey != NULL)
			{
				RegCloseKey(hSubRegKey);
				hSubRegKey = NULL;
			}

			++dwIndex;
			nKeyNameBuffSize = 32767;
		}
	} while (0);

	if (pKeyName != NULL)
	{
		free(pKeyName);
		pKeyName = NULL;
	}

	if (pClassName != NULL)
	{
		free(pClassName);
		pClassName = NULL;
	}

	if (hRegKey != NULL)
	{
		RegCloseKey(hRegKey);
		hRegKey = NULL;
	}
}

void CDriverToolFrame::UpdateDriverInfo()
{

	wxString keyPath = L"SYSTEM\\CurrentControlSet\\Services\\" + m_szServiceName;

	HKEY hMainKey = NULL;
	ULONG ulRet = 0;
	DWORD dwStart = 3;
	DWORD dwRetSize;
	DWORD dwValueType = 0;
	do
	{
		if (m_szServiceName == wxT(""))
		{
			SetStatusText(wxT("请选择驱动文件"));
			break;
		}
		ulRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, keyPath.c_str(), 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hMainKey);
		if (ulRet != ERROR_SUCCESS)
		{
			SetStatusText(wxT("注册表访问失败，请确定服务已经安装或该程序有足够高权限"));
			break;
		}
		
		dwRetSize = 4;
		ulRet = RegQueryValueEx(hMainKey, TEXT("Start"), 0, NULL, (const PBYTE)&dwStart, &dwRetSize);
		if (ulRet != ERROR_SUCCESS)
		{
			SetStatusText(wxT("Start键值访问失败"));
			break;
		}
		m_pRadioBoxStartOption->SetSelection(dwStart);
		RegCloseKey(hMainKey);
		hMainKey = NULL;

		TCHAR szDriverList[1024];

		auto iter = g_GUIDMap.cbegin();
		for (int index = 0; iter != g_GUIDMap.cend(); ++iter, ++index)
		{
			keyPath = TEXT("SYSTEM\\CurrentControlSet\\Control\\Class\\") + iter->second;
																						 
			ulRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, keyPath.c_str(), 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hMainKey);
			if (ulRet != ERROR_SUCCESS)
			{
				continue;
			}

			dwRetSize = 2048;
			ulRet = RegQueryValueEx(hMainKey, TEXT("UpperFilters"), 0, NULL, (const PBYTE)szDriverList, &dwRetSize);
			if (ulRet != ERROR_SUCCESS)
			{
				continue;
			}

			TCHAR *p = szDriverList;
			while (*p != 0)
			{
				if (!_tcsicmp(m_szServiceName.c_str(), p))
				{
					m_ppCheckBoxArray[index]->SetValue(true);
					break;
				}

				p += _tcslen(p) + 1;
			}

		}

		SetStatusText(wxT("设备信息获取完毕"));
	} while (0);

	if (hMainKey != NULL)
	{
		RegCloseKey(hMainKey);
	}
}

char * CDriverToolFrame::FindRes(DWORD dwResId, PDWORD pResSize)
{
	char *p = nullptr;
	HRSRC hExe = NULL;

	do 
	{
		hExe = FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(dwResId), RT_BITMAP);
		if (hExe == NULL)
		{
			assert(false);
			break;
		}

		DWORD dwResSize = SizeofResource(GetModuleHandle(NULL), hExe);
		if (dwResSize == 0)
		{
			break;
		}

		if (pResSize != nullptr)
		{
			*pResSize = dwResSize;
		}

		HGLOBAL hData = LoadResource(GetModuleHandle(NULL), hExe);
		if (hData == NULL)
		{
			break;
		}

		LPVOID pData = LockResource(hData);
		if (pData == NULL)
		{
			break;
		}

		p = static_cast<char*>(pData);
	} while (0);

	return p;
}

void CDriverToolFrame::UpdateIoctlInfo(IOCTL_INFO & ioctlInfo)
{
	m_pIoctlControl->UpdateIoctlInfo(ioctlInfo);
}

void CDriverToolFrame::NotifyExtFrame()
{
	wxString szPath;
	szPath = m_pEdtDriverPath->GetLabelText();
	CServiceControl sc;
	m_szServiceName = sc.FindServiceName(szPath.c_str());
	UpdateDriverInfo();
}

void CDriverToolFrame::ClearCheckBox()
{
	for (unsigned i = 0; i < g_GUIDMap.size(); ++i)
	{
		m_ppCheckBoxArray[i]->SetValue(false);
	}
}
