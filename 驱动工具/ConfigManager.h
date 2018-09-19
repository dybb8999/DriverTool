#pragma once
#include <string>

class CConfigManager
{
private:
	std::wstring m_strLastUsedFile;

private:
	HKEY m_hKey;
	static CConfigManager *m_pInstance;

public:
	CConfigManager();
	~CConfigManager();

	static CConfigManager* GetInstances();

	void LoadConfig();

	
	bool SetLastUsedFile(std::wstring filePath);
	std::wstring GetLastUsedFile();
private:
	void InitConfig();
	bool OpenReg();

	bool SaveData(std::wstring ValueName, std::wstring ValueData);

	std::wstring GetData(std::wstring ValueName);
};

