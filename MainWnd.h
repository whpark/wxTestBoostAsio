﻿#pragma once

#ifndef __MainWnd__
#define __MainWnd__

/**
@file
Subclass of IMainWnd, which is generated by wxFormBuilder.
*/

#include "wxUI.h"

/** Implementing IMainWnd */
class xMainWnd : public ui::IMainWnd {
public:
	using this_t = xMainWnd;
	using base_t = ui::IMainWnd;

protected:
	bool m_bInitialized{};

public:
	/** Constructor */
	xMainWnd(wxWindow* parent);

protected:
	// server
	boost::asio::io_context m_io_context;
	//std::shared_ptr<boost::asio::io_context::strand> m_strand;

	// listener
	std::optional<std::jthread> m_listener;
	bool m_bListenerStopped{};

protected:
	virtual void OnMove( wxMoveEvent& event ) override;
	virtual void OnSize( wxSizeEvent& event ) override;
	virtual void OnTimer_UpdateUI( wxTimerEvent& event ) override;

	virtual void OnChkListen( wxCommandEvent& event ) override;
	virtual void OnConnectTo( wxCommandEvent& event ) override;

};

#endif // __MainWnd__
