#include "stdafx.h"
#include "ConfigManager.h"

CConfigManager* CConfigManager::m_pInstance = nullptr;

CConfigManager::CConfigManager()
{
}


CConfigManager::~CConfigManager()
{
}

CConfigManager * CConfigManager::GetInstances()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CConfigManager;
	}
	return m_pInstance;
}

void CConfigManager::LoadConfig()
{
	do 
	{
		if (OpenReg() == false)
		{
			break;
		}

		m_strLastUsedFile = GetData(L"LastUsedFile");

	} while (false);
	
}

bool CConfigManager::SetLastUsedFile(std::wstring filePath)
{
	if (m_strLastUsedFile != filePath)
	{
		m_strLastUsedFile = filePath;
		SaveData(L"LastUsedFile", filePath);
	}
	return true;
}

std::wstring CConfigManager::GetLastUsedFile()
{
	return m_strLastUsedFile;
}

void CConfigManager::InitConfig()
{
	do
	{
		if (OpenReg() == false)
		{
			break;
		}

		SaveData(L"LastUsedFile", L"");

	} while (false);
}

bool CConfigManager::OpenReg()
{
	bool bRet = false;
	LSTATUS lStatus = 0;

	HKEY hKey = nullptr;
	do 
	{
		if (m_hKey != nullptr)
		{
			bRet = true;
			break;
		}

		lStatus = RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\DriverTool", NULL, KEY_ALL_ACCESS, &hKey);
		if (lStatus != 0)
		{
			if (lStatus == 2)
			{
				DWORD dwResult = 0;
				lStatus = RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\DriverTool", NULL, NULL, NULL, KEY_ALL_ACCESS, NULL, &hKey, &dwResult);
				if (lStatus != 0)
				{
					break;
				}

				InitConfig();
			}
			else
			{
				break;
			}
		}

		m_hKey = hKey;

		bRet = true;
	} while (false);

	return bRet;
}

bool CConfigManager::SaveData(std::wstring ValueName, std::wstring ValueData)
{
	bool bRet = false;
	LSTATUS lStatus = 0;

	do 
	{
		if (m_hKey == nullptr)
		{
			break;
		}

		lStatus = RegSetValueExW(m_hKey, ValueName.c_str(), NULL, REG_SZ, (const BYTE*)ValueData.c_str(), (ValueData.length() + 1) * sizeof(wchar_t));
		if (lStatus != 0)
		{
			break;
		}

		bRet = true;
	} while (false);

	return bRet;
}

std::wstring CConfigManager::GetData(std::wstring ValueName)
{
	std::wstring bRet = L"";
	LSTATUS lStatus = 0;

	wchar_t *p = nullptr;
	do
	{
		if (m_hKey == nullptr)
		{
			break;
		}

		DWORD dwDataSize = 0;

		lStatus = RegQueryValueExW(m_hKey, ValueName.c_str(), NULL, NULL, NULL, &dwDataSize);
		if (lStatus != 0)
		{
			break;
		}

		p = (wchar_t *)malloc(dwDataSize);
		if (p == nullptr)
		{
			break;
		}

		lStatus = RegQueryValueExW(m_hKey, ValueName.c_str(), NULL, NULL, (LPBYTE)p, &dwDataSize);
		if (lStatus != 0)
		{
			break;
		}

		bRet = p;

	} while (false);

	if (p != nullptr)
	{
		free(p);
		p = nullptr;
	}

	return bRet;
}
