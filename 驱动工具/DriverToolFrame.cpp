#include "stdafx.h"
#include "DriverToolFrame.h"
#include "ServiceControl.h"
#include "ControlWorkerThread.h"
#include "ExtendedFrame.h"
#include "DefEvent.h"
#include <wx\\collpane.h>
#include <wx\\stattext.h>
#include <map>

wxDEFINE_EVENT(wxEVT_MY_CUSTOM_COMMAND, wxCommandEvent);


wxBEGIN_EVENT_TABLE(CDriverToolFrame, wxFrame)
	EVT_BUTTON(ID_BTN_SELECTPATH, CDriverToolFrame::OnSelectFile)
	EVT_BUTTON(ID_BTN_INSTALL, CDriverToolFrame::OnInstall)
	EVT_BUTTON(ID_BTN_START, CDriverToolFrame::OnStart)
	EVT_BUTTON(ID_BTN_STOP, CDriverToolFrame::OnStop)
	EVT_BUTTON(ID_BTN_UNINSTALL, CDriverToolFrame::OnUnInstall)
	EVT_THREAD(THREAD_SERVER_CONTROL_COMPLETE, CDriverToolFrame::OnServiceControlComplete)
	EVT_CHECKBOX(ID_CHK_WINDOWTOP, CDriverToolFrame::OnWindowTop)
	EVT_BUTTON(ID_BTN_MORE, CDriverToolFrame::OnShowExtendFrame)
	EVT_DROP_FILES(CDriverToolFrame::OnDropFile)
	//EVT_MOVE(OnWindowMove)
wxEND_EVENT_TABLE()

std::map<wxString, wxString> g_GUIDMap;

CDriverToolFrame::CDriverToolFrame() :wxFrame(NULL, wxID_ANY, wxT("驱动工具"), wxDefaultPosition, wxSize(610, 200), wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX | wxCAPTION | wxCLIP_CHILDREN)
{
	InitWDMFilterData();
	wxFont font;
	font.Create(13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("微软雅黑"));
	SetIcon(wxICON(icon));
	m_pPanel = new wxPanel(this, wxID_ANY);
	m_pPanel->SetBackgroundColour(wxColor(255, 255, 255));
	this->DragAcceptFiles(true);

	m_pMainBoxSizer = new wxBoxSizer(wxVERTICAL);

	m_pStaticBoxTopSizer = new wxStaticBoxSizer(wxHORIZONTAL, m_pPanel, wxT("文件"));

	m_pStaticPath = new wxStaticText(m_pPanel, wxID_ANY, wxT("文件路径:"));
	m_pStaticPath->SetFont(font);
	m_pStaticBoxTopSizer->Add(m_pStaticPath, 0, wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL | wxALL, 5);

	m_pEdtDriverPath = new wxTextCtrl(m_pPanel, ID_EDIT_FILEPATH);
	m_pEdtDriverPath->SetFont(font);
	m_pStaticBoxTopSizer->Add(m_pEdtDriverPath, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_pBtnSelectPath = new wxButton(m_pPanel, ID_BTN_SELECTPATH, wxT("..."), wxDefaultPosition, wxSize(50,30));
	m_pStaticBoxTopSizer->Add(m_pBtnSelectPath, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 5);

	m_pChkBoxWindowTop = new wxCheckBox(m_pPanel, ID_CHK_WINDOWTOP, wxT("窗口置顶"));
	m_pChkBoxWindowTop->SetFont(font);
	m_pStaticBoxTopSizer->Add(m_pChkBoxWindowTop, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 5);

	m_pMiddleStaticBoxSizer = new wxStaticBoxSizer(wxHORIZONTAL, m_pPanel, wxT("控制"));
	m_pBtnInstall = new wxButton(m_pPanel, ID_BTN_INSTALL, wxT("安装"));
	m_pBtnInstall->SetFont(font);
	m_pMiddleStaticBoxSizer->Add(m_pBtnInstall, 1, wxALIGN_CENTRE_VERTICAL | wxALL, 5);
	
	m_pBtnStart = new wxButton(m_pPanel, ID_BTN_START, wxT("启动"));
	m_pBtnStart->SetFont(font);
	m_pMiddleStaticBoxSizer->Add(m_pBtnStart, 1, wxALIGN_CENTRE_VERTICAL | wxALL, 5);

	m_pBtnStop = new wxButton(m_pPanel, ID_BTN_STOP, wxT("停止"));
	m_pBtnStop->SetFont(font);
	m_pMiddleStaticBoxSizer->Add(m_pBtnStop, 1, wxALIGN_CENTRE_VERTICAL | wxALL, 5);

	m_pBtnUninstall = new wxButton(m_pPanel, ID_BTN_UNINSTALL, wxT("卸载"));
	m_pBtnUninstall->SetFont(font);
	m_pMiddleStaticBoxSizer->Add(m_pBtnUninstall, 1, wxALIGN_CENTRE_VERTICAL | wxALL, 5);

	m_pBottomBoxSizer = new wxStaticBoxSizer(wxHORIZONTAL, m_pPanel, wxT("状态"));
	m_pStaticStatus = new wxStaticText(m_pPanel, wxID_ANY, wxT("状态:"));
	m_pStaticStatus->SetFont(font);
	m_pBottomBoxSizer->Add(m_pStaticStatus, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_pEdtShow = new wxTextCtrl(m_pPanel, ID_EDT_SHOW, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
	m_pEdtShow->SetFont(font);
	m_pBottomBoxSizer->Add(m_pEdtShow, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_pStaticName = new wxStaticText(m_pPanel, wxID_ANY, wxT("By.Hell"));
	m_pStaticName->SetFont(font);
	m_pBottomBoxSizer->Add(m_pStaticName, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_pOtherBoxSizer = new wxStaticBoxSizer(wxHORIZONTAL, m_pPanel, wxT("其他"));
	//InitCollapsiblePane();

	m_pBtnMore = new wxButton(m_pPanel, ID_BTN_MORE, wxT("更多"));
	m_pOtherBoxSizer->Add(m_pBtnMore, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_pGauge = new wxGauge(m_pPanel, ID_GAUGE, 100);
	//m_pGauge->Pulse();
	m_pOtherBoxSizer->Add(m_pGauge, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	//Extend Panel
	

	m_pMainBoxSizer->Add(m_pStaticBoxTopSizer, 1, wxEXPAND, 10);
	m_pMainBoxSizer->Add(m_pMiddleStaticBoxSizer, 1, wxEXPAND, 10);
	m_pMainBoxSizer->Add(m_pBottomBoxSizer, 1, wxEXPAND, 10);
	m_pMainBoxSizer->Add(m_pOtherBoxSizer, 1, wxEXPAND, 10);

	m_pExtendFrame = new CExtendedFrame(this, ID_EXTEND_FRAME, wxT("更多"));
	m_pExtendFrame->Show(false);

	m_pPanel->SetSizer(m_pMainBoxSizer);
	m_pMainBoxSizer->SetMinSize(wxSize(600, 100));
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

		CServiceControl sc;

		//wxWidgets 没办法自由发消息，因此全部改为异步调用
		m_pExtendFrame->SetServiceName(wxString(sc.FindServiceName(szPath.c_str())));
		m_pExtendFrame->CallAfter(&CExtendedFrame::OnUpdateInfo);
	} while (0);
}

void CDriverToolFrame::OnInstall(wxCommandEvent & event)
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

void CDriverToolFrame::OnShowExtendFrame(wxCommandEvent & event)
{
	wxRect rect = this->GetRect();
	wxPoint point;
	point.x = rect.GetRight();
	point.y = rect.GetTop();

	m_pExtendFrame->Move(point);
	m_pExtendFrame->Show(true);

	m_pExtendFrame->CallAfter(&CExtendedFrame::OnUpdateInfo);
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
}

void CDriverToolFrame::OnDropFile(wxDropFilesEvent & event)
{
	if (event.GetNumberOfFiles() > 0)
	{
		m_pEdtDriverPath->SetLabelText(event.GetFiles()[0]);
		CServiceControl sc;
		m_pExtendFrame->SetServiceName(wxString(sc.FindServiceName(event.GetFiles()[0].c_str())));
		m_pExtendFrame->CallAfter(&CExtendedFrame::OnUpdateInfo);
	}

	
}

void CDriverToolFrame::InitCollapsiblePane()
{
	//Init map
	{
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Battery"), wxT("{72631e54-78a4-11d0-bcf7-00aa00b7b32a}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Biometric"), wxT("{53D29EF7-377C-4D14-864B-EB3A85769359}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Bluetooth"), wxT("{e0cbf06c-cd8b-4647-bb8a-263b43f0f974}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("CDROM"), wxT("{4d36e965-e325-11ce-bfc1-08002be10318}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("DiskDrive"), wxT("{4d36e967-e325-11ce-bfc1-08002be10318}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Display"), wxT("{4d36e968-e325-11ce-bfc1-08002be10318}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Extension"), wxT("{e2f84ce7-8efa-411c-aa69-97454ca4cb57}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("FDC"), wxT("{4d36e969-e325-11ce-bfc1-08002be10318}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("FloppyDisk"), wxT("{4d36e980-e325-11ce-bfc1-08002be10318}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("GPS"), wxT("{6bdd1fc3-810f-11d0-bec7-08002be2092f}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("HDC"), wxT("{4d36e96a-e325-11ce-bfc1-08002be10318}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("HIDClass"), wxT("{745a17a0-74d3-11d0-b6fe-00a0c90f57da}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Dot4"), wxT("{48721b56-6795-11d2-b1a8-0080c72e74a2}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Dot4Print"), wxT("{49ce6ac8-6f86-11d2-b1e5-0080c72e74a2}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("61883"), wxT("{7ebefbc0-3200-11d2-b4c2-00a0C9697d07}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("AVC"), wxT("{c06ff265-ae09-48f0-812c-16753d7cba83}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("SBP2"), wxT("{d48179be-ec20-11d1-b6b8-00c04fa372a7}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("1394"), wxT("{6bdd1fc1-810f-11d0-bec7-08002be2092f}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Image"), wxT("{6bdd1fc6-810f-11d0-bec7-08002be2092f}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Infrared"), wxT("{6bdd1fc5-810f-11d0-bec7-08002be2092f}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Keyboard"), wxT("{4d36e96b-e325-11ce-bfc1-08002be10318}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("MediumChanger"), wxT("{ce5939ae-ebde-11d0-b181-0000f8753ec4}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("MTD"), wxT("{4d36e970-e325-11ce-bfc1-08002be10318}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Modem"), wxT("{4d36e96d-e325-11ce-bfc1-08002be10318}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Monitor"), wxT("{4d36e96e-e325-11ce-bfc1-08002be10318}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Mouse"), wxT("{4d36e96f-e325-11ce-bfc1-08002be10318}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Multifunction"), wxT("{4d36e971-e325-11ce-bfc1-08002be10318}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Media"), wxT("{4d36e96c-e325-11ce-bfc1-08002be10318}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("MultiportSerial"), wxT("{50906cb8-ba12-11d1-bf5d-0000f805f530}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Net"), wxT("{4d36e972-e325-11ce-bfc1-08002be10318}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("NetClient"), wxT("{4d36e973-e325-11ce-bfc1-08002be10318}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("NetService"), wxT("{4d36e974-e325-11ce-bfc1-08002be10318}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("NetTrans"), wxT("{4d36e975-e325-11ce-bfc1-08002be10318}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("SecurityAccelerator"), wxT("{268c95a1-edfe-11d3-95c3-0010dc4050a5}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("PCMCIA"), wxT("{4d36e977-e325-11ce-bfc1-08002be10318}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Ports"), wxT("{4d36e978-e325-11ce-bfc1-08002be10318}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Printer"), wxT("{4d36e979-e325-11ce-bfc1-08002be10318}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("PNPPrinters"), wxT("{4658ee7e-f050-11d1-b6bd-00c04fa372a7}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Processor"), wxT("{50127dc3-0f36-415e-a6cc-4cb3be910b65}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("SCSIAdapter"), wxT("{4d36e97b-e325-11ce-bfc1-08002be10318}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Sensor"), wxT("{5175d334-c371-4806-b3ba-71fd53c9258d}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("SmartCardReader"), wxT("{50dd5230-ba8a-11d1-bf5d-0000f805f530}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("SoftwareComponent"), wxT("{5c4c3332-344d-483c-8739-259e934c9cc8}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Volume"), wxT("{71a27cdd-812a-11d0-bec7-08002be2092f}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("System"), wxT("{4d36e97d-e325-11ce-bfc1-08002be10318}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("TapeDrive"), wxT("{6d807884-7d21-11cf-801c-08002be10318}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("USBDevice"), wxT("{88BAE032-5A81-49f0-BC3D-A4FF138216D6}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("WCEUSBS"), wxT("{25dbce51-6c8f-4a72-8a6d-b54c2b4fc835}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("WPD"), wxT("{eec5ad98-8080-425f-922a-dabf3de3f69a}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("SideShow"), wxT("{997b5d8d-c442-4f2e-baf3-9c8e671e9e21}")));
		g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Camera"), wxT("{ca3e7ab9-b4c3-4ae6-8251-579ef933890f}")));
	}
	wxWindow *pMainPanel;
	m_pExtendPanel = new wxCollapsiblePane(m_pPanel, wxID_ANY, wxT("更多"));
	pMainPanel = m_pExtendPanel->GetPane();
	m_pOtherBoxSizer->Add(m_pExtendPanel, 0, wxGROW | wxALL, 5);

	m_pExtenPanelMainBoxSizer = new wxBoxSizer(wxVERTICAL);
	m_pExtBoxSize1 = new wxBoxSizer(wxVERTICAL);
	//wxStaticText* pStatic = new wxStaticText(pMainPanel, wxID_ANY, wxT("Test"));
	//m_pExtBoxSize1->Add(pStatic);
	//pMainPanel->SetSizer(m_pExtBoxSize1);
	//m_pExtBoxSize1->SetSizeHints(pMainPanel);
	
	m_pNotebook = new wxNotebook(pMainPanel, ID_NOTEBOOK);
	
	//Panel 1
	wxArrayString strRadioBoxs;
	strRadioBoxs.Add(wxT("BOOT_START"));
	strRadioBoxs.Add(wxT("SYSTEM_START"));
	strRadioBoxs.Add(wxT("AUTO_START"));
	strRadioBoxs.Add(wxT("DEMAND_START"));
	strRadioBoxs.Add(wxT("DISABLED"));

	m_pFirstPanel = new wxPanel(m_pNotebook);
	m_pNotebook->AddPage(m_pFirstPanel, wxT("详细设置"), true);

	m_pRadioBoxStartOption = new wxRadioBox(m_pFirstPanel, ID_RADIOBOX_PAGE1, wxT("启动方式"), wxDefaultPosition, wxDefaultSize, strRadioBoxs, 0, wxRA_SPECIFY_COLS);
	//m_pRadioBoxStartOption->SetSelection(3);
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

	//Panel 2
	m_pExtSecondPanel = new wxPanel(m_pNotebook);
	m_pNotebook->AddPage(m_pExtSecondPanel, wxT("其他"));
	
	m_pFirstPanel->SetSizer(m_pExtBoxSize1);
	m_pExtenPanelMainBoxSizer->Add(m_pNotebook, 1, wxEXPAND | wxALL, 5);
	pMainPanel->SetSizer(m_pExtenPanelMainBoxSizer);
	m_pExtenPanelMainBoxSizer->SetSizeHints(pMainPanel);
	
}

void CDriverToolFrame::InitWDMFilterData()
{
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Battery"), wxT("{72631e54-78a4-11d0-bcf7-00aa00b7b32a}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Biometric"), wxT("{53D29EF7-377C-4D14-864B-EB3A85769359}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Bluetooth"), wxT("{e0cbf06c-cd8b-4647-bb8a-263b43f0f974}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("CDROM"), wxT("{4d36e965-e325-11ce-bfc1-08002be10318}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("DiskDrive"), wxT("{4d36e967-e325-11ce-bfc1-08002be10318}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Display"), wxT("{4d36e968-e325-11ce-bfc1-08002be10318}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Extension"), wxT("{e2f84ce7-8efa-411c-aa69-97454ca4cb57}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("FDC"), wxT("{4d36e969-e325-11ce-bfc1-08002be10318}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("FloppyDisk"), wxT("{4d36e980-e325-11ce-bfc1-08002be10318}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("GPS"), wxT("{6bdd1fc3-810f-11d0-bec7-08002be2092f}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("HDC"), wxT("{4d36e96a-e325-11ce-bfc1-08002be10318}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("HIDClass"), wxT("{745a17a0-74d3-11d0-b6fe-00a0c90f57da}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Dot4"), wxT("{48721b56-6795-11d2-b1a8-0080c72e74a2}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Dot4Print"), wxT("{49ce6ac8-6f86-11d2-b1e5-0080c72e74a2}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("61883"), wxT("{7ebefbc0-3200-11d2-b4c2-00a0C9697d07}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("AVC"), wxT("{c06ff265-ae09-48f0-812c-16753d7cba83}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("SBP2"), wxT("{d48179be-ec20-11d1-b6b8-00c04fa372a7}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("1394"), wxT("{6bdd1fc1-810f-11d0-bec7-08002be2092f}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Image"), wxT("{6bdd1fc6-810f-11d0-bec7-08002be2092f}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Infrared"), wxT("{6bdd1fc5-810f-11d0-bec7-08002be2092f}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Keyboard"), wxT("{4d36e96b-e325-11ce-bfc1-08002be10318}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("MediumChanger"), wxT("{ce5939ae-ebde-11d0-b181-0000f8753ec4}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("MTD"), wxT("{4d36e970-e325-11ce-bfc1-08002be10318}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Modem"), wxT("{4d36e96d-e325-11ce-bfc1-08002be10318}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Monitor"), wxT("{4d36e96e-e325-11ce-bfc1-08002be10318}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Mouse"), wxT("{4d36e96f-e325-11ce-bfc1-08002be10318}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Multifunction"), wxT("{4d36e971-e325-11ce-bfc1-08002be10318}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Media"), wxT("{4d36e96c-e325-11ce-bfc1-08002be10318}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("MultiportSerial"), wxT("{50906cb8-ba12-11d1-bf5d-0000f805f530}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Net"), wxT("{4d36e972-e325-11ce-bfc1-08002be10318}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("NetClient"), wxT("{4d36e973-e325-11ce-bfc1-08002be10318}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("NetService"), wxT("{4d36e974-e325-11ce-bfc1-08002be10318}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("NetTrans"), wxT("{4d36e975-e325-11ce-bfc1-08002be10318}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("SecurityAccelerator"), wxT("{268c95a1-edfe-11d3-95c3-0010dc4050a5}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("PCMCIA"), wxT("{4d36e977-e325-11ce-bfc1-08002be10318}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Ports"), wxT("{4d36e978-e325-11ce-bfc1-08002be10318}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Printer"), wxT("{4d36e979-e325-11ce-bfc1-08002be10318}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("PNPPrinters"), wxT("{4658ee7e-f050-11d1-b6bd-00c04fa372a7}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Processor"), wxT("{50127dc3-0f36-415e-a6cc-4cb3be910b65}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("SCSIAdapter"), wxT("{4d36e97b-e325-11ce-bfc1-08002be10318}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Sensor"), wxT("{5175d334-c371-4806-b3ba-71fd53c9258d}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("SmartCardReader"), wxT("{50dd5230-ba8a-11d1-bf5d-0000f805f530}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("SoftwareComponent"), wxT("{5c4c3332-344d-483c-8739-259e934c9cc8}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Volume"), wxT("{71a27cdd-812a-11d0-bec7-08002be2092f}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("System"), wxT("{4d36e97d-e325-11ce-bfc1-08002be10318}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("TapeDrive"), wxT("{6d807884-7d21-11cf-801c-08002be10318}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("USBDevice"), wxT("{88BAE032-5A81-49f0-BC3D-A4FF138216D6}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("WCEUSBS"), wxT("{25dbce51-6c8f-4a72-8a6d-b54c2b4fc835}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("WPD"), wxT("{eec5ad98-8080-425f-922a-dabf3de3f69a}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("SideShow"), wxT("{997b5d8d-c442-4f2e-baf3-9c8e671e9e21}")));
	g_GUIDMap.insert(std::pair<wxString, wxString>(wxT("Camera"), wxT("{ca3e7ab9-b4c3-4ae6-8251-579ef933890f}")));

}
