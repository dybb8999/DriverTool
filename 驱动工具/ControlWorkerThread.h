#pragma once

enum 
{
	THREAD_SERVER_CONTROL_COMPLETE
};

class CDriverToolFrame;

class CControlWorkerThread :
	public wxThread
{
public:
	CControlWorkerThread(CDriverToolFrame *pFrame);
	~CControlWorkerThread();

	virtual wxThread::ExitCode Entry() wxOVERRIDE;

	virtual void OnExit() wxOVERRIDE;

public:
	ULONG m_ulMessageID;
	CDriverToolFrame *m_pFrame;
};

