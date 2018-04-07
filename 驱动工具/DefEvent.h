#pragma once
#include "wx\\event.h"

class EventUpdateExtendFrameInfo : public wxEvent
{
public:
	EventUpdateExtendFrameInfo(wxString &serviceName);

	virtual wxEvent *Clone() const wxOVERRIDE { return new EventUpdateExtendFrameInfo(*this); }

public:
	wxString m_strServiceName;
};

wxDECLARE_EVENT(EXTENDFRAMEUPDATE, EventUpdateExtendFrameInfo);

typedef void (wxEvtHandler::*MyPlotEventFunction)(EventUpdateExtendFrameInfo&);
#define MyPlotEventHandler(func) wxEVENT_HANDLER_CAST(MyPlotEventFunction, func)

#define MY_EVT_PLOT_CLICK(id, func) \
    wx__DECLARE_EVT1(EXTENDFRAMEUPDATE, id, MyPlotEventHandler(func))