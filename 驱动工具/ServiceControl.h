#pragma once
class CServiceControl
{
public:
	CServiceControl();
	CServiceControl(TCHAR* filePath);
	~CServiceControl();

	void SetPath(const TCHAR* filePath);
	BOOL InstallService();
	BOOL RunService();
	BOOL StopService();
	BOOL UnInstallService();
	TCHAR* FindServiceName(const TCHAR* filePath);

	TCHAR* GetErrorStr(DWORD errorCode);
	DWORD GetErrorCode() { return m_errorCode; };
	BOOL ImageCheck();
public:
	TCHAR m_filePath[MAX_PATH];
	TCHAR m_serviceName[MAX_PATH];
	DWORD m_errorCode;
};

