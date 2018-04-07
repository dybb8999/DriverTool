#pragma once

class CMyApp :
	public wxApp
{
public:
	CMyApp();
	~CMyApp();
	bool OnInit() wxOVERRIDE;
};

