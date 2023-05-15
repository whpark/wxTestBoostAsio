﻿#ifndef __wxUIIClient__
#define __wxUIIClient__

/**
@file
Subclass of IClient, which is generated by wxFormBuilder.
*/

#include "wxUI.h"

#include "Comm.h"


/** Implementing IClient */
class xWorkerWnd : public ui::IWorkerWnd {
public:
	/** Constructor */
	xWorkerWnd(wxWindow* parent, std::string ip, int port);
	~xWorkerWnd();

protected:
	// worker
	std::optional<xChatClient> m_client;
	asio::io_context m_io_context;

	virtual void OnTimer_UpdateUI( wxTimerEvent& event ) override;
	virtual void OnTextEnter_Message( wxCommandEvent& event ) override;
	virtual void OnTextEnter_Parameters( wxCommandEvent& event ) override;
	virtual void OnButtonClick_Send( wxCommandEvent& event ) override;
	virtual void OnButtonClick_Close( wxCommandEvent& event ) override;


	void OnEvtIPComm(xEvtIPComm& event);
};

#endif // __wxUIIClient__
