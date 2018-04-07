#include "stdafx.h"
#include "DefEvent.h"

wxDEFINE_EVENT(UpdateExtendFrame, EventUpdateExtendFrameInfo);

EventUpdateExtendFrameInfo::EventUpdateExtendFrameInfo(wxString & serviceName) : wxEvent()
{
	m_strServiceName = serviceName;
}
