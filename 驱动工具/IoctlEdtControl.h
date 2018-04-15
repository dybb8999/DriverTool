#pragma once
#include "DriverToolFrame.h"
#include <map>

class wxTextCtrl;

class CIoctlEdtControl
{
private:
	wxTextCtrl* m_pEdtCode;
	wxTextCtrl* m_pEdtMnemonic;
	wxTextCtrl* m_pEdtDevType;
	wxTextCtrl* m_pEdtFunction;
	wxTextCtrl* m_pEdtMethod;
	wxTextCtrl* m_pEdtAccess;
	wxTextCtrl* m_pEdtLayout;

	std::map<wxString, ULONG> m_mapMnemonicToCode;
	std::map<ULONG, wxString> m_mapCodeAndMnemonic;
	std::map<ULONG, wxString> m_mapDeviceType;
	//std::map<ULONG, wxString> m_mapFunction;
	std::map<ULONG, wxString> m_mapMethod;
	std::map<ULONG, wxString> m_mapAccess;
public:
	CIoctlEdtControl(wxTextCtrl * code, wxTextCtrl * mnemonic, wxTextCtrl * devType, wxTextCtrl * function, wxTextCtrl * method, wxTextCtrl * access, wxTextCtrl * layout);
	~CIoctlEdtControl();
	void InitMapData();
	ULONG_PTR GetMnemonicNumber();

	void MnemonicSet(const wxString &strMnemonic);
	void GetIoctlInfo(IOCTL_INFO &ioctlInfo);
	void UpdateIoctlInfo(const IOCTL_INFO& ioctlInfo);
};

