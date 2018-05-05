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
	_tcscpy_s(m_serviceName, FindServiceName(m_filePath));
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

BOOL CServiceControl::ImageCheck()
{
	DWORD dwRet = 0;
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
			break;
		}

		fread_s(buff, 1024, 1024, 1, fp);

		fclose(fp);
		fp = NULL;

		PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)buff;
		if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE)
		{
			break;
		}

		PIMAGE_NT_HEADERS32 ntHeaders = (PIMAGE_NT_HEADERS32)(buff + dosHeader->e_lfanew - 1);
		if (ntHeaders->Signature != IMAGE_NT_SIGNATURE)
		{
			break;
		}

		BOOL bIsWow64Process = FALSE;
		if (IsWow64Process(GetCurrentProcess(), &bIsWow64Process) == 0)
		{
			break;
		}

		if (ntHeaders->FileHeader.Machine == IMAGE_FILE_MACHINE_I386 && bIsWow64Process == TRUE)
		{
			break;
		}

		if (ntHeaders->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64 && bIsWow64Process == FALSE)
		{
			break;
		}

		dwRet = 1;
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

	return dwRet;
}
