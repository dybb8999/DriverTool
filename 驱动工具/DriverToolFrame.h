#pragma once
#include <map>
#include <vector>
#include <wx\\collpane.h>
#include <wx\\notebook.h>
#include <wx\\spinbutt.h>


enum 
{
	ID_EDIT_FILEPATH = 1,
	ID_BTN_SELECTPATH,
	ID_CHK_WINDOWTOP,
	ID_BTN_INSTALL,
	ID_BTN_START,
	ID_BTN_STOP,
	ID_BTN_UNINSTALL,
	ID_EDT_SHOW,
	ID_BTN_MORE,
	ID_GAUGE,
	ID_EXTEND_FRAME,
	//CollapsiblePane
	ID_COLLAPSIBLEPANE,
	ID_NOTEBOOK,
	ID_RADIOBOX_PAGE1,
	ID_RADIOBEGIN,
	ID_RADIOEND = ID_RADIOBEGIN + 50,
	ID_EDTSHOWIOCTL,
	ID_EDTSHOWMNEMONIC,//m_pEdtMnemonic
	ID_SPIL_DEVICETYPE,
	ID_SPIL_FUNCTION,
	ID_SPIL_METHOD
};

enum ServiceControlCode
{
	INSTALL,
	START,
	STOP,
	UNINSTALL
};

typedef union _IOCTL_INFO
{
	ULONG ulData;
	struct
	{
		ULONG Method : 2;
		ULONG Function : 12;
		ULONG Access : 2;
		ULONG DeviceType : 16;
	}Info;
}IOCTL_INFO, *PIOCTL_INFO;

class CIoctlEdtControl;

class CDriverToolFrame :
	public wxFrame
{
	friend class CControlWorkerThread;
public:
	CDriverToolFrame();
	~CDriverToolFrame();

private:
	CIoctlEdtControl *m_pIoctlControl;
	std::vector<wxSizer*> m_vecSizerPointArray;
	wxPanel* m_pPanel;

	wxBoxSizer* m_pMainBoxSizer;
	
	wxStaticBoxSizer* m_pStaticBoxTopSizer;
	wxBoxSizer* m_pTopBoxSizer;
	wxStaticText* m_pStaticPath;
	wxTextCtrl* m_pEdtDriverPath;
	wxCheckBox* m_pChkBoxWindowTop;
	wxButton* m_pBtnSelectPath;

	//wxBoxSizer* m_pMiddleBoxSizer;
	wxStaticBoxSizer* m_pMiddleStaticBoxSizer;
	wxButton* m_pBtnInstall;
	wxButton* m_pBtnStart;
	wxButton* m_pBtnStop;
	wxButton* m_pBtnUninstall;

	//wxBoxSizer* m_pBottomBoxSizer;
	wxStaticBoxSizer *m_pBottomBoxSizer;
	wxStaticText* m_pStaticStatus;
	wxTextCtrl* m_pEdtShow;
	wxStaticText* m_pStaticName;
	

	//wxBoxSizer* m_pOtherBoxSizer;
	wxStaticBoxSizer* m_pOtherBoxSizer;
	wxButton* m_pBtnMore;
	wxGauge* m_pGauge;

	//CExtendedFrame* m_pExtendFrame;

	wxBoxSizer* m_pCollapsibleSizer;
	wxCollapsiblePane* m_pExtendPanel;
	//ExtendPanel
	wxBoxSizer *m_pExtenPanelMainBoxSizer;
	wxNotebook *m_pNotebook;

	wxPanel *m_pFirstPanel;
	wxBoxSizer *m_pExtBoxSize1;
	wxRadioBox *m_pRadioBoxStartOption;
	wxStaticBoxSizer *m_pBottomStaticBoxSizer;
	wxGridSizer *m_pCheckBoxSizer;
	wxCheckBox **m_ppCheckBoxArray;

	//SecondPanel
	wxFlexGridSizer *m_pFlexGridSizer;
	wxPanel *m_pExtSecondPanel;
	wxBoxSizer *m_pExtBoxSize2;
	wxStaticBoxSizer *m_pIoctlInfoSizer;
	wxStaticBoxSizer *m_pIoctlSetailsSizer;
	wxStaticBoxSizer *m_pIoctlLayout;

	wxStaticText *m_pStaticIoctlNumber;
	wxTextCtrl *m_pEdtShowIoctlCode;
	wxStaticText *m_pStaticNumberOfIoctl;

	wxStaticText *m_pStaticMnemonic;
	wxTextCtrl *m_pEdtMnemonic;
	
	wxStaticText *m_pStaticDeviceType;
	wxTextCtrl *m_pEdtDeviceType;
	wxSpinButton *m_pSpinDeviceType;
	
	wxStaticText *m_pStaticFunction;
	wxTextCtrl *m_pEdtFunction;
	wxSpinButton *m_pSpinFunction;

	wxStaticText *m_pStaticMethod;
	wxTextCtrl *m_pEdtMethod;
	wxSpinButton *m_pSpinMethod;

	wxStaticText *m_pStaticAccess;
	wxTextCtrl *m_pEdtAccess;
	wxSpinButton *m_pSpinAccess;

	wxTextCtrl *m_pEdtIoctlLayout;
	wxStaticBitmap *m_pShowIoctlLayout;

	wxString m_szServiceName;
private:
	void OnSelectFile(wxCommandEvent &event);
	void OnInstall(wxCommandEvent &event);
	void OnStart(wxCommandEvent &event);
	void OnStop(wxCommandEvent &event);
	void OnUnInstall(wxCommandEvent &event);
	void OnWindowTop(wxCommandEvent &event);
	void OnWindowMove(wxMoveEvent &event);
	//void OnShowExtendFrame(wxCommandEvent &event);
	void OnStartChange(wxCommandEvent & event);
	void OnFilterDriverNotify(wxCommandEvent & event);

	void DisableAllButton();
	void EnableAllButton();
	void OnServiceControlComplete(wxThreadEvent& event);
	void OnDropFile(wxDropFilesEvent& event);
	void OnCollapsiblePaneExpand(wxCollapsiblePaneEvent& event);

	void OnIoctlCodeChange(wxCommandEvent& event);
private:
	void InitCollapsiblePane();
	void InitWDMFilterData();
	void UpdateDriverInfo();
	char* FindRes(DWORD dwResId, PDWORD pResSize = nullptr);
	void UpdateIoctlInfo(IOCTL_INFO& ioctlInfo);
private:
	wxDECLARE_EVENT_TABLE();
};

extern std::map<wxString, wxString> g_GUIDMap;