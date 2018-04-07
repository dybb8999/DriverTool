#include "stdafx.h"
#include "ExtendedFrame.h"
#include <map>
#include "DefEvent.h"
#include "DriverToolFrame.h"
#include <Windows.h>

wxBEGIN_EVENT_TABLE(CExtendedFrame, wxFrame)
	EVT_RADIOBOX(ID_RADIOBOX_PAGE1, CExtendedFrame::OnStartChange)
	EVT_CHECKBOX(ID_RADIOBEGIN + 0, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 1, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 2, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 3, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 4, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 5, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 6, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 7, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 8, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 9, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 10, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 11, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 12, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 13, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 14, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 15, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 16, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 17, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 18, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 19, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 20, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 21, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 22, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 23, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 24, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 25, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 26, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 27, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 28, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 29, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 30, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 31, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 32, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 33, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 34, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 35, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 36, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 37, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 38, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 39, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 40, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 41, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 42, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 43, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 44, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 45, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 46, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 47, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 48, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 49, CExtendedFrame::OnFilterDriverNotify)
	EVT_CHECKBOX(ID_RADIOBEGIN + 50, CExtendedFrame::OnFilterDriverNotify)
	EVT_CLOSE(OnClose)
wxEND_EVENT_TABLE()

extern std::map<wxString, wxString> g_GUIDMap;

CExtendedFrame::CExtendedFrame(wxWindow * parent, wxWindowID id, const wxString & title, const wxPoint & pos, const wxSize & size, long style, const wxString & name):
	wxFrame(parent, id, title, pos, size, wxSYSTEM_MENU | wxCLOSE_BOX | wxCAPTION | wxCLIP_CHILDREN, name)
{
	m_szServiceName = wxT("");
	SetIcon(wxICON(icon));
	CreateStatusBar();

	m_pMainPanel = new wxPanel(this);
	m_pMainBoxSizer = new wxBoxSizer(wxVERTICAL);
	m_pNotebook = new wxNotebook(m_pMainPanel, ID_NOTEBOOK);

	//Panel 1
	wxArrayString strRadioBoxs;
	strRadioBoxs.Add(wxT("BOOT_START"));
	strRadioBoxs.Add(wxT("SYSTEM_START"));
	strRadioBoxs.Add(wxT("AUTO_START"));
	strRadioBoxs.Add(wxT("DEMAND_START"));
	strRadioBoxs.Add(wxT("DISABLED"));

	m_pFirstPanel = new wxPanel(m_pNotebook);
	m_pNotebook->AddPage(m_pFirstPanel, wxT("详细设置"), true);

	m_pMainBoxSizer1 = new wxBoxSizer(wxVERTICAL);
	m_pRadioBox = new wxRadioBox(m_pFirstPanel, ID_RADIOBOX_PAGE1, wxT("启动方式"), wxDefaultPosition, wxDefaultSize, strRadioBoxs, 0, wxRA_SPECIFY_COLS);
	m_pRadioBox->SetSelection(3);
	m_pMainBoxSizer1->Add(m_pRadioBox, 0, wxALL | wxEXPAND, 5);

	m_pBottomStaticBoxSizer = new wxStaticBoxSizer(wxVERTICAL, m_pFirstPanel, wxT("附加设备(非WDM驱动勿选)"));
	m_pGridSizer = new wxGridSizer(0, 4, 0, 0);
	m_pBottomStaticBoxSizer->Add(m_pGridSizer, 1, wxALL | wxEXPAND, 5);
	m_pMainBoxSizer1->Add(m_pBottomStaticBoxSizer, 1, wxALL | wxEXPAND, 5);

	m_ppCheckBoxArray = new wxCheckBox*[g_GUIDMap.size()];
	auto iter = g_GUIDMap.cbegin();
	for (int index = 0; iter != g_GUIDMap.cend(); ++iter, ++index)
	{
		m_ppCheckBoxArray[index] = new wxCheckBox(m_pFirstPanel, ID_RADIOBEGIN + index, iter->first);
		m_pGridSizer->Add(m_ppCheckBoxArray[index]);
	}

	//Panel 2
	m_pSecondPanel = new wxPanel(m_pNotebook);
	m_pNotebook->AddPage(m_pSecondPanel, wxT("其他"));

	m_pFirstPanel->SetSizer(m_pMainBoxSizer1);
	m_pMainBoxSizer->Add(m_pNotebook, 1, wxEXPAND | wxALL, 5);
	m_pMainPanel->SetSizer(m_pMainBoxSizer);
	m_pMainBoxSizer->SetSizeHints(this);
}

CExtendedFrame::~CExtendedFrame()
{
}

void CExtendedFrame::OnUpdateInfo()
{
	wxString keyPath = L"SYSTEM\\CurrentControlSet\\Services\\" + m_szServiceName;

	HKEY hMainKey = NULL;
	ULONG ulRet = 0;
	DWORD dwStart = 3;
	DWORD dwRetSize;
	do 
	{
		ulRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, keyPath.c_str(), 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hMainKey);
		if (ulRet != ERROR_SUCCESS)
		{
			SetStatusText(wxT("注册表访问失败，请确定服务已经安装或该程序有足够高权限"));
			break;
		}

		dwRetSize = 4;
		ulRet = RegGetValue(hMainKey, NULL, TEXT("Start"), RRF_RT_REG_DWORD, NULL, &dwStart, &dwRetSize);
		if (ulRet != ERROR_SUCCESS)
		{
			SetStatusText(wxT("Start键值访问失败"));
			break;
		}
		m_pRadioBox->SetSelection(dwStart);
		RegCloseKey(hMainKey);
		hMainKey = NULL;

		TCHAR szDriverList[1024];

		auto iter = g_GUIDMap.cbegin();
		for (int index = 0; iter != g_GUIDMap.cend(); ++iter, ++index)
		{
			keyPath = TEXT("SYSTEM\\CurrentControlSet\\Control\\Class\\") + iter->second;//{ca3e7ab9-b4c3-4ae6-8251-579ef933890f}
			//keyPath = TEXT("SYSTEM\\CurrentControlSet\\Control\\Class\\{ca3e7ab9-b4c3-4ae6-8251-579ef933890f}");
			ulRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, keyPath.c_str(), 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hMainKey);
			if (ulRet != ERROR_SUCCESS)
			{
				continue;
			}

			dwRetSize = 2048;
			ulRet = RegGetValue(hMainKey, NULL, TEXT("UpperFilters"), RRF_RT_REG_MULTI_SZ | RRF_RT_REG_SZ, NULL, szDriverList, &dwRetSize);
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

void CExtendedFrame::SetServiceName(const wxString & name)
{
	m_szServiceName = name;
}

void CExtendedFrame::OnStartChange(wxCommandEvent & event)
{
	int nSelect = m_pRadioBox->GetSelection();
	wxString keyPath = L"SYSTEM\\CurrentControlSet\\Services\\" + m_szServiceName;

	HKEY hMainKey = NULL;
	ULONG ulRet = 0;
	do 
	{
		if (m_szServiceName == wxT(""))
		{
			SetStatusText(wxT("请选择一个文件"));
			m_pRadioBox->SetSelection(3);
			break;
		}

		ulRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, keyPath.c_str(), 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hMainKey);
		if (ulRet != ERROR_SUCCESS)
		{
			SetStatusText(wxT("注册表访问失败，请确定服务已经安装或该程序有足够高权限"));
			break;
		}

		ulRet = sizeof(DWORD);
		ulRet = RegSetKeyValue(hMainKey, NULL, TEXT("Start"), REG_DWORD, &nSelect, sizeof(DWORD));
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

void CExtendedFrame::OnFilterDriverNotify(wxCommandEvent & event)
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
		ulRet = RegGetValue(hMainKey, NULL, TEXT("UpperFilters"), RRF_RT_REG_MULTI_SZ | RRF_RT_REG_SZ, NULL, szDriverList, &dwRetSize);
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
			bool bFind = false;
			TCHAR *p = szDriverList;
			while (*p != 0)
			{
				if (!_tcsicmp(m_szServiceName.c_str(), p))
				{
					bFind = true;
					break;
				}

				p += _tcslen(p) + 1;
			}

			if (bFind == true)
			{
				//找到了？再见
				break;
			}

			_tcscpy_s(p, 1024 - (p - szDriverList), m_szServiceName.c_str());
			p += _tcslen(p) + 1;
			*p = 0;

			ulRet = RegSetValueEx(hMainKey, TEXT("UpperFilters"), 0, REG_MULTI_SZ, (CONST BYTE*)szDriverList, (p - szDriverList) * sizeof(TCHAR));
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
				/*
				if (!_tcsicmp(m_szServiceName.c_str(), p))
				{
					bFind = true;
					break;
				}
				*/
				p += szItem.Length() + 1;
			}

			/*
			if (bFind == false)
			{
				//没找到，本来就没有
				break;
			}
			*/
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
				
				p += 1;
				length += 1;
			}
			//memcpy(p, p + _tcslen(p) + 1, dwRetSize - (p + _tcslen(p) + 1 - szDriverList));
			/*
			p = szDriverList;
			while (*p != 0)
			{
				if (!_tcsicmp(m_szServiceName.c_str(), p))
				{
					bFind = true;
					break;
				}

				p += _tcslen(p) + 1;
			}
			*/
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

void CExtendedFrame::OnClose(wxCloseEvent & event)
{
	this->Show(false);
}
