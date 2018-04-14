#pragma once
#include <wx\\collpane.h>
#include <wx\\notebook.h>
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
	ID_RADIOEND = ID_RADIOBEGIN + 50
};

enum ServiceControlCode
{
	INSTALL,
	START,
	STOP,
	UNINSTALL
};

//class CExtendedFrame;

class CDriverToolFrame :
	public wxFrame
{
	friend class CControlWorkerThread;
public:
	CDriverToolFrame();
	~CDriverToolFrame();

private:
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

	wxPanel *m_pExtSecondPanel;

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
private:
	void InitCollapsiblePane();
	void InitWDMFilterData();
	void UpdateDriverInfo();
private:
	wxDECLARE_EVENT_TABLE();
};

