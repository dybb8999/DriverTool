#include "stdafx.h"
#include "ServiceControl.h"
#include <winsvc.h>


CServiceControl::CServiceControl()
{
}

CServiceControl::CServiceControl(TCHAR* filePath):m_errorCode(0)
{
	_tcscpy_s(m_filePath, filePath);
	_tcscpy_s(m_serviceName, FindServiceName(m_filePath));
}

CServiceControl::~CServiceControl()
{
}

void CServiceControl::SetPath(const TCHAR * filePath)
{
	_tcscpy_s(m_filePath, filePath);
	auto strServiceName = FindServiceName(m_filePath);
	if (strServiceName != nullptr)
	{
		_tcscpy_s(m_serviceName, strServiceName);
	}
}

BOOL CServiceControl::InstallService()
{
	SC_HANDLE sc_manage = NULL;
	SC_HANDLE sc_service = NULL;
	BOOL bRet = FALSE;

	do 
	{
		sc_manage = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
		if (NULL == sc_manage)
		{
			m_errorCode = GetLastError();
			break;
		}

		sc_service = CreateService(
			sc_manage, m_serviceName, m_serviceName, SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_IGNORE,
			m_filePath, NULL, NULL, NULL, NULL, NULL);
		if (NULL == sc_service)
		{
			m_errorCode = GetLastError();
			break;
		}

		bRet = TRUE;
	} while (0);

	if (sc_service != NULL)
	{
		CloseServiceHandle(sc_service);
		sc_service = NULL;
	}

	if (sc_manage != NULL)
	{
		CloseServiceHandle(sc_manage);
		sc_manage = NULL;
	}

	return bRet;
}

BOOL CServiceControl::RunService()
{
	SC_HANDLE sc_manage = NULL;
	SC_HANDLE sc_service = NULL;
	BOOL bRet = FALSE;

	do 
	{
		sc_manage = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (NULL == sc_manage)
		{
			m_errorCode = GetLastError();
			break;
		}

		sc_service = OpenService(sc_manage, m_serviceName, SERVICE_ALL_ACCESS);
		if (NULL == sc_service)
		{
			m_errorCode = GetLastError();
			break;
		}

		if (StartService(sc_service, NULL, NULL) == FALSE)
		{
			m_errorCode = GetLastError();
			break;
		}

		bRet = TRUE;
	} while (0);

	if (sc_service != NULL)
	{
		CloseServiceHandle(sc_service);
		sc_service = NULL;
	}

	if (sc_manage != NULL)
	{
		CloseServiceHandle(sc_manage);
		sc_manage = NULL;
	}

	return bRet;
}

BOOL CServiceControl::StopService()
{
	SC_HANDLE sc_manage = NULL;
	SC_HANDLE sc_service = NULL;
	SERVICE_STATUS ss = { 0 };
	BOOL bRet = FALSE;

	do 
	{
		sc_manage = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (NULL == sc_manage)
		{
			m_errorCode = GetLastError();
			break;
		}

		sc_service = OpenService(sc_manage, m_serviceName, SERVICE_ALL_ACCESS);
		if (NULL == sc_service)
		{
			m_errorCode = GetLastError();
			break;
		}

		if (ControlService(sc_service, SERVICE_CONTROL_STOP, &ss) == FALSE)
		{
			m_errorCode = GetLastError();
			break;
		}

		bRet = TRUE;
	} while (0);
	
	if (sc_service != NULL)
	{
		CloseServiceHandle(sc_service);
		sc_service = NULL;
	}

	if (sc_manage != NULL)
	{
		CloseServiceHandle(sc_manage);
		sc_manage = NULL;
	}

	return bRet;
}

BOOL CServiceControl::UnInstallService()
{
	SC_HANDLE sc_manage = NULL;
	SC_HANDLE sc_service = NULL;
	BOOL bRet = FALSE;

	do 
	{
		sc_manage = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (sc_manage == NULL)
		{
			m_errorCode = GetLastError();
			break;
		}

		sc_service = OpenService(sc_manage, m_serviceName, SERVICE_ALL_ACCESS);
		if (sc_service == NULL)
		{
			m_errorCode = GetLastError();
			break;
		}

		if (DeleteService(sc_service) == FALSE)
		{
			m_errorCode = GetLastError();
			break;
		}

		bRet = TRUE;
	} while (0);
	
	if (sc_service != NULL)
	{
		CloseServiceHandle(sc_service);
		sc_service = NULL;
	}

	if (sc_manage != NULL)
	{
		CloseServiceHandle(sc_manage);
		sc_manage = NULL;
	}

	return bRet;
}

TCHAR * CServiceControl::FindServiceName(const TCHAR * filePath)
{
	if (_tcslen(filePath) == 0)
	{
		return nullptr;
	}

	TCHAR szFilePath[1024];
	_tcscpy_s(szFilePath, filePath);
	TCHAR *fp1 = _tcsrchr(szFilePath, '\\') + 1;
	int i = 0;
	for (; (fp1[i] != TEXT('\0')) && (fp1[i] != TEXT('.')); ++i)
	{
		m_serviceName[i] = fp1[i];
	}
	m_serviceName[i] = TEXT('\0');
	return m_serviceName;
}

TCHAR * CServiceControl::GetErrorStr(DWORD errorCode)
{
	static TCHAR strBuff[1024] = TEXT("No text found for this error number.");
	DWORD dwError = errorCode;

	HLOCAL hlocal = NULL;   // Buffer that gets the error message string

							// Use the default system locale since we look for Windows messages.
							// Note: this MAKELANGID combination has 0 as value
	DWORD systemLocale = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);

	// Get the error code's textual description
	BOOL fOk = FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS |
		FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL, dwError, systemLocale,
		(PTSTR)&hlocal, 0, NULL);

	if (!fOk) {
		// Is it a network-related error?
		HMODULE hDll = LoadLibraryEx(TEXT("netmsg.dll"), NULL,
			DONT_RESOLVE_DLL_REFERENCES);

		if (hDll != NULL) {
			fOk = FormatMessage(
				FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS |
				FORMAT_MESSAGE_ALLOCATE_BUFFER,
				hDll, dwError, systemLocale,
				(PTSTR)&hlocal, 0, NULL);
			FreeLibrary(hDll);
		}
	}

	if (fOk && (hlocal != NULL)) {
		_tcscpy_s(strBuff, (TCHAR*)hlocal);
		LocalFree(hlocal);
	}

	return strBuff;
}

ULONG CServiceControl::ImageCheck()
{
	ULONG ulRet = 0;
	unsigned char* buff = NULL;
	FILE *fp = NULL;
	do
	{
		buff = (unsigned char*)calloc(1024, sizeof(unsigned char));
		if (buff == NULL)
		{
			break;
		}

		if (_tfopen_s(&fp, m_filePath, L"r") != 0)
		{
			ulRet = -1;
			break;
		}

		fread_s(buff, 1024, 1024, 1, fp);

		fclose(fp);
		fp = NULL;

		PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)buff;
		if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE)
		{
			ulRet = -2;
			break;
		}

		PIMAGE_NT_HEADERS32 ntHeaders = (PIMAGE_NT_HEADERS32)(buff + dosHeader->e_lfanew - 1);
		if (ntHeaders->Signature != IMAGE_NT_SIGNATURE)
		{
			ulRet = -3;
			break;
		}

		BOOL bIsWow64Process = FALSE;
		if (IsWow64Process(GetCurrentProcess(), &bIsWow64Process) == 0)
		{
			ulRet = -4;
			break;
		}

		if (ntHeaders->FileHeader.Machine == IMAGE_FILE_MACHINE_I386 && bIsWow64Process == TRUE)
		{
			ulRet = -5;
			break;
		}

		if (ntHeaders->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64 && bIsWow64Process == FALSE)
		{
			ulRet = -5;
			break;
		}

		ulRet = 0;
	} while (0);

	if (buff != NULL)
	{
		free(buff);
		buff = NULL;
	}

	if (fp != NULL)
	{
		fclose(fp);
		fp = NULL;
	}

	return ulRet;
}

DWORD CServiceControl::MinifilterSupport()
{
	DWORD dwRet = 0;
	LSTATUS lStatus = 0;

	HKEY hKey = nullptr;
	HKEY hKeyInstance = nullptr;
	HKEY hKeyInstanceSub = nullptr;

	wchar_t szRegSubPath[256];

	do
	{
		swprintf_s(szRegSubPath, L"SYSTEM\\CurrentControlSet\\Services\\%s", m_serviceName);

		lStatus = RegOpenKeyExW(
			HKEY_LOCAL_MACHINE,
			szRegSubPath,
			NULL,
			KEY_ALL_ACCESS | KEY_WOW64_64KEY,
			&hKey);
		if (ERROR_SUCCESS != lStatus)
		{
			dwRet = -1;
			break;
		}

		//添加DependOnService REG_MULTI_SZ
		lStatus = RegSetValueExW(
			hKey,
			L"DependOnService",
			0,
			REG_MULTI_SZ,
			(PBYTE)L"FltMgr\0\0",
			sizeof(L"FltMgr\0\0"));
		if (ERROR_SUCCESS != lStatus)
		{
			dwRet = -1;
			break;
		}

		lStatus = RegSetValueExW(
			hKey,
			L"Group",
			0,
			REG_SZ,
			(PBYTE)L"FSFilter Activity Monitor",
			sizeof(L"FSFilter Activity Monitor"));
		if (ERROR_SUCCESS != lStatus)
		{
			dwRet = -1;
			break;
		}

		DWORD dwCrateDisposition = 0;
		//创建子项 Instances
		lStatus = RegCreateKeyExW(
			hKey,
			L"Instances",
			0,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS,
			NULL,
			&hKeyInstance,
			&dwCrateDisposition);
		if (ERROR_SUCCESS != lStatus)
		{
			dwRet = -1;
			break;
		}

		std::wstring wstrInstanceName = m_serviceName;
		wstrInstanceName += L" Instance";

		lStatus = RegSetValueExW(
			hKeyInstance,
			L"DefaultInstance",
			0,
			REG_SZ,
			(const PBYTE)wstrInstanceName.c_str(),
			(wstrInstanceName.length() + 1) * sizeof(wchar_t));
		if (ERROR_SUCCESS != lStatus)
		{
			dwRet = -1;
			break;
		}

		lStatus = RegCreateKeyExW(
			hKeyInstance,
			wstrInstanceName.c_str(),
			0,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS,
			NULL,
			&hKeyInstanceSub,
			&dwCrateDisposition
		);
		if (ERROR_SUCCESS != lStatus)
		{
			dwRet = -1;
			break;
		}

		lStatus = RegSetValueExW(
			hKeyInstanceSub,
			L"Altitude",
			0,
			REG_SZ,
			(const PBYTE)L"370090",
			sizeof(L"370090"));
		if (ERROR_SUCCESS != lStatus)
		{
			dwRet = -1;
			break;
		}

		DWORD dwFlages = 0;
		lStatus = RegSetValueExW(
			hKeyInstanceSub,
			L"Flags",
			0,
			REG_DWORD,
			(const PBYTE)&dwFlages,
			sizeof(dwFlages));
		if (ERROR_SUCCESS != lStatus)
		{
			dwRet = -1;
			break;
		}

	} while (false);

	if (hKeyInstance != nullptr)
	{
		RegCloseKey(hKeyInstance);
		hKeyInstance = nullptr;
	}

	if (hKeyInstanceSub != nullptr)
	{
		RegCloseKey(hKeyInstanceSub);
		hKeyInstanceSub = nullptr;
	}

	return dwRet;
}
