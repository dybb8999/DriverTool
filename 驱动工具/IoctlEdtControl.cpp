#include "stdafx.h"
#include "IoctlEdtControl.h"
#include <wx/textctrl.h>
#include <wx/string.h>
#include <map>
#include <bitset>

CIoctlEdtControl::CIoctlEdtControl(wxTextCtrl * code, wxTextCtrl * mnemonic, wxTextCtrl * devType, wxTextCtrl * function, wxTextCtrl * method, wxTextCtrl * access, wxTextCtrl * layout):
	m_pEdtCode(code), m_pEdtMnemonic(mnemonic), m_pEdtDevType(devType), m_pEdtFunction(function), m_pEdtMethod(method), m_pEdtAccess(access), m_pEdtLayout(layout)
{
	InitMapData();
}

CIoctlEdtControl::~CIoctlEdtControl()
{
}

void CIoctlEdtControl::InitMapData()
{
#define INSERT(mapdata, first)\
	mapdata.insert(std::pair<ULONG, wxString>(first, wxT(#first)))

	//Access
	INSERT(m_mapAccess, FILE_ANY_ACCESS);
	INSERT(m_mapAccess, FILE_READ_ACCESS);
	INSERT(m_mapAccess, FILE_WRITE_ACCESS);
	INSERT(m_mapAccess, FILE_READ_ACCESS+FILE_WRITE_ACCESS);

	//Method
	INSERT(m_mapMethod, METHOD_BUFFERED);
	INSERT(m_mapMethod, METHOD_IN_DIRECT);
	INSERT(m_mapMethod, METHOD_OUT_DIRECT);
	INSERT(m_mapMethod, METHOD_NEITHER);

	//DeviceType
	{
		INSERT(m_mapDeviceType, FILE_DEVICE_BEEP);
		INSERT(m_mapDeviceType, FILE_DEVICE_CD_ROM);
		INSERT(m_mapDeviceType, FILE_DEVICE_CD_ROM_FILE_SYSTEM);
		INSERT(m_mapDeviceType, FILE_DEVICE_CONTROLLER);
		INSERT(m_mapDeviceType, FILE_DEVICE_DATALINK);
		INSERT(m_mapDeviceType, FILE_DEVICE_DFS);
		INSERT(m_mapDeviceType, FILE_DEVICE_DISK);
		INSERT(m_mapDeviceType, FILE_DEVICE_DISK_FILE_SYSTEM);
		INSERT(m_mapDeviceType, FILE_DEVICE_FILE_SYSTEM);
		INSERT(m_mapDeviceType, FILE_DEVICE_INPORT_PORT);
		INSERT(m_mapDeviceType, FILE_DEVICE_KEYBOARD);
		INSERT(m_mapDeviceType, FILE_DEVICE_MAILSLOT);
		INSERT(m_mapDeviceType, FILE_DEVICE_MIDI_IN);
		INSERT(m_mapDeviceType, FILE_DEVICE_MIDI_OUT);
		INSERT(m_mapDeviceType, FILE_DEVICE_MOUSE);
		INSERT(m_mapDeviceType, FILE_DEVICE_MULTI_UNC_PROVIDER);
		INSERT(m_mapDeviceType, FILE_DEVICE_NAMED_PIPE);
		INSERT(m_mapDeviceType, FILE_DEVICE_NETWORK);
		INSERT(m_mapDeviceType, FILE_DEVICE_NETWORK_BROWSER);
		INSERT(m_mapDeviceType, FILE_DEVICE_NETWORK_FILE_SYSTEM);
		INSERT(m_mapDeviceType, FILE_DEVICE_NULL);
		INSERT(m_mapDeviceType, FILE_DEVICE_PARALLEL_PORT);
		INSERT(m_mapDeviceType, FILE_DEVICE_PHYSICAL_NETCARD);
		INSERT(m_mapDeviceType, FILE_DEVICE_PRINTER);
		INSERT(m_mapDeviceType, FILE_DEVICE_SCANNER);
		INSERT(m_mapDeviceType, FILE_DEVICE_SERIAL_MOUSE_PORT);
		INSERT(m_mapDeviceType, FILE_DEVICE_SERIAL_PORT);
		INSERT(m_mapDeviceType, FILE_DEVICE_SCREEN);
		INSERT(m_mapDeviceType, FILE_DEVICE_SOUND);
		INSERT(m_mapDeviceType, FILE_DEVICE_STREAMS);
		INSERT(m_mapDeviceType, FILE_DEVICE_TAPE);
		INSERT(m_mapDeviceType, FILE_DEVICE_TAPE_FILE_SYSTEM);
		INSERT(m_mapDeviceType, FILE_DEVICE_TRANSPORT);
		INSERT(m_mapDeviceType, FILE_DEVICE_UNKNOWN);
		INSERT(m_mapDeviceType, FILE_DEVICE_VIDEO);
		INSERT(m_mapDeviceType, FILE_DEVICE_VIRTUAL_DISK);
		INSERT(m_mapDeviceType, FILE_DEVICE_WAVE_IN);
		INSERT(m_mapDeviceType, FILE_DEVICE_WAVE_OUT);
		INSERT(m_mapDeviceType, FILE_DEVICE_8042_PORT);
		INSERT(m_mapDeviceType, FILE_DEVICE_NETWORK_REDIRECTOR);
		INSERT(m_mapDeviceType, FILE_DEVICE_BATTERY);
		INSERT(m_mapDeviceType, FILE_DEVICE_BUS_EXTENDER);
		INSERT(m_mapDeviceType, FILE_DEVICE_MODEM);
		INSERT(m_mapDeviceType, FILE_DEVICE_VDM);
		INSERT(m_mapDeviceType, FILE_DEVICE_MASS_STORAGE);
		INSERT(m_mapDeviceType, FILE_DEVICE_SMB);
		INSERT(m_mapDeviceType, FILE_DEVICE_KS);
		INSERT(m_mapDeviceType, FILE_DEVICE_CHANGER);
		INSERT(m_mapDeviceType, FILE_DEVICE_SMARTCARD);
		INSERT(m_mapDeviceType, FILE_DEVICE_ACPI);
		INSERT(m_mapDeviceType, FILE_DEVICE_DVD);
		INSERT(m_mapDeviceType, FILE_DEVICE_FULLSCREEN_VIDEO);
		INSERT(m_mapDeviceType, FILE_DEVICE_DFS_FILE_SYSTEM);
		INSERT(m_mapDeviceType, FILE_DEVICE_DFS_VOLUME);
		INSERT(m_mapDeviceType, FILE_DEVICE_SERENUM);
		INSERT(m_mapDeviceType, FILE_DEVICE_TERMSRV);
		INSERT(m_mapDeviceType, FILE_DEVICE_KSEC);
		INSERT(m_mapDeviceType, FILE_DEVICE_FIPS);
		INSERT(m_mapDeviceType, FILE_DEVICE_INFINIBAND);
		INSERT(m_mapDeviceType, FILE_DEVICE_VMBUS);
		INSERT(m_mapDeviceType, FILE_DEVICE_CRYPT_PROVIDER);
		INSERT(m_mapDeviceType, FILE_DEVICE_WPD);
		INSERT(m_mapDeviceType, FILE_DEVICE_BLUETOOTH);
		INSERT(m_mapDeviceType, FILE_DEVICE_MT_COMPOSITE);
		INSERT(m_mapDeviceType, FILE_DEVICE_MT_TRANSPORT);
		INSERT(m_mapDeviceType, FILE_DEVICE_BIOMETRIC);
		INSERT(m_mapDeviceType, FILE_DEVICE_PMI);

	}
	
#undef INSERT
}

ULONG_PTR CIoctlEdtControl::GetMnemonicNumber()
{
	return m_mapCodeAndMnemonic.size();
}



void CIoctlEdtControl::GetIoctlInfo(IOCTL_INFO & ioctlInfo)
{
	wxString strCode = m_pEdtCode->GetLabelText();
	_tcscanf_s(strCode.c_str(), TEXT("%x"), &ioctlInfo.ulData);
}

void CIoctlEdtControl::UpdateIoctlInfo(const IOCTL_INFO & ioctlInfo)
{
	do 
	{
		if (ioctlInfo.ulData == 0)
		{
			m_pEdtDevType->SetLabelText(wxT(""));
			m_pEdtFunction->SetLabelText(wxT(""));
			m_pEdtMethod->SetLabelText(wxT(""));
			m_pEdtAccess->SetLabelText(wxT(""));
			m_pEdtLayout->SetLabelText(wxT(""));
			//m_pEdtCode->SetLabelText(wxT("0x00000000"));
			m_pEdtCode->SetInsertionPoint(-1);
			m_pEdtLayout->SetLabelText(wxT("00000000000000000000000000000000"));
			break;;
		}

		wxString strTemp;
		strTemp = wxString::Format(wxT("%08X"), ioctlInfo.ulData);
		m_pEdtCode->SetLabelText(strTemp);
		m_pEdtCode->SetInsertionPoint(-1);

		//DeviceType
		auto p1 = m_mapDeviceType.find(ioctlInfo.Info.DeviceType);
		if (p1 != m_mapDeviceType.end())
		{
			m_pEdtDevType->SetValue(p1->second);
		}
		else
		{
			m_pEdtDevType->SetValue(wxT(""));
		}

		//Function
		strTemp = wxString::Format(wxT("0x%X"), ioctlInfo.Info.Function);
		m_pEdtFunction->SetLabelText(strTemp);

		//Method
		auto p2 = m_mapMethod.find(ioctlInfo.Info.Method);
		if (p2 != m_mapMethod.end())
		{
			m_pEdtMethod->SetValue(p2->second);
		}
		else
		{
			m_pEdtMethod->SetValue(wxT(""));
		}

		//Access
		auto p3 = m_mapAccess.find(ioctlInfo.Info.Access);
		if (p3 != m_mapAccess.end())
		{
			m_pEdtAccess->SetValue(p3->second);
		}
		else
		{
			m_pEdtAccess->SetValue(wxT(""));
		}

		//DeviceType
		auto p4 = m_mapDeviceType.find(ioctlInfo.Info.DeviceType);
		if (p4 != m_mapDeviceType.end())
		{
			m_pEdtDevType->SetValue(p4->second);
		}
		else
		{
			m_pEdtDevType->SetValue(wxT("0"));
		}

		std::bitset<32> bitData(ioctlInfo.ulData);
		m_pEdtLayout->SetLabelText(bitData.to_string());
	} while (0);
	
}
