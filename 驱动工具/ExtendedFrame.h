#pragma once

#include "wx\\notebook.h"
#include "DefEvent.h"

enum 
{
	ID_NOTEBOOK = 1,
	ID_RADIOBOX_PAGE1,
	ID_RADIOBEGIN,
	ID_RADIOEND = ID_RADIOBEGIN + 50
};


class CExtendedFrame :
	public wxFrame
{
public:
	CExtendedFrame(wxWindow *parent,
		wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_FRAME_STYLE,
		const wxString& name = wxFrameNameStr);
	~CExtendedFrame();

	void OnUpdateInfo();
	void SetServiceName(const wxString &name);
	void OnStartChange(wxCommandEvent& event);
	void OnFilterDriverNotify(wxCommandEvent& event);
	void OnClose(wxCloseEvent& event);
private:
	wxPanel *m_pMainPanel;
	wxBoxSizer *m_pMainBoxSizer;
	wxNotebook *m_pNotebook;

	wxPanel *m_pFirstPanel;
	wxBoxSizer *m_pMainBoxSizer1;
	wxRadioBox *m_pRadioBox;
	wxStaticBoxSizer *m_pBottomStaticBoxSizer;
	wxGridSizer *m_pGridSizer;
	wxCheckBox **m_ppCheckBoxArray;

	wxPanel *m_pSecondPanel;

	wxString m_szServiceName;
private:
	wxDECLARE_EVENT_TABLE();
};