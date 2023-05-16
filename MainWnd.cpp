#include "pch.h"
#include "wxDesktopApp.h"
#include "MainWnd.h"
#include "WorkerWnd.h"
#include "gtl/wx/util.h"


xMainWnd::xMainWnd( wxWindow* parent ) : ui::IMainWnd( parent ) {
	gtl::wx::LoadWindowPosition(wxGetApp().m_reg, "MainWindow"s, this);
	m_bInitialized = true;
}

void xMainWnd::OnMove(wxMoveEvent& event) {
	if (!m_bInitialized)
		return;
	gtl::wx::SaveWindowPosition(wxGetApp().m_reg, "MainWindow"s, this);
}

void xMainWnd::OnSize(wxSizeEvent& event) {
	//event.Skip();
	wxFrame::OnSize(event);
	if (!m_bInitialized)
		return;
	gtl::wx::SaveWindowPosition(wxGetApp().m_reg, "MainWindow"s, this);
}

void xMainWnd::OnTimer_UpdateUI(wxTimerEvent& event) {
	if (!m_bInitialized)
		return;
	// Garbage Collection, listener
	if (m_listener and m_bListenerStopped and m_listener->joinable()) {
		m_listener.reset();
	}

	auto UpdateChk = [&](auto* ctrl, bool b) {
		if (!ctrl)
			return;
		if (ctrl->IsChecked()) {
			if (!b)
				ctrl->Set3StateValue(wxCheckBoxState::wxCHK_UNCHECKED);
		}
		else {
			if (b)
				ctrl->Set3StateValue(wxCheckBoxState::wxCHK_CHECKED);
		}
	};

	UpdateChk(ui_chkListener, (bool)m_listener);
	UpdateChk(ui_chkListenerTS, (bool)m_serverTS);
	UpdateChk(ui_chkListenerUDP, m_serverUDP.IsRuning());

	std::string str;
	if (m_serverTS) {
		size_t nClients{};
		{
			std::shared_lock lock(m_serverTS->m_mtxSockets);
			nClients = m_serverTS->m_sockets.size();
		}

		str += std::format("ServerTS Clients : {}\n", nClients);

	}
	if (ui_textStatus->GetValue() != wxString(str)) {
		ui_textStatus->SetValue(str);
	}

}

void xMainWnd::OnChkListen(wxCommandEvent& event) {
	if (!m_listener) {
		// start listening

		int port = std::atoi(ui_textPort->GetValue().c_str());

		m_bListenerStopped = false;
		m_listener = std::jthread([port, this](std::stop_token st) {
			{
				if (m_io_context.stopped())
					m_io_context.restart();
				//asio::io_context io_context;
				xChatServer server(m_io_context, port);

				Log("Listener thread started");
				m_io_context.run();
				//while (!st.stop_requested()) {
				//	m_io_context.poll_one();
				//	std::this_thread::yield();
				//}
				//std::this_thread::sleep_for(1000ms);
				Log("Listener thread stopped");
			}
			m_bListenerStopped = true;
		});

	} else {
		// stop listening
		//if (!m_listener)
		//	return;
		m_io_context.stop();
		m_listener->request_stop();
	}
}

void xMainWnd::OnChkListenTS(wxCommandEvent& event) {
	if (!m_serverTS) {
		// start listening

		int port = std::atoi(ui_textPortTS->GetValue().c_str());

		m_serverTS.emplace();
		m_serverTS->Start(port);

	} else {
		m_serverTS->Stop();
		m_serverTS.reset();
	}
}

void xMainWnd::OnChkListenUDP(wxCommandEvent& event) {
	if (m_serverUDP.IsRuning()) {
		m_serverUDP.Stop();
	} else {
		int port = std::atoi(ui_textPortUDP->GetValue().c_str());
		m_serverUDP.Start(port);
	}
}

template < bool bUDP >
bool xMainWnd::ConnectTo(wxString const& url) {
	// split url into ip and port
	auto pos = url.find(':');
	if (pos == url.npos) {
		Log("Invalid URL");
		return false;
	}
	std::string ip = url.substr(0, pos).ToStdString();
	auto port = atoi(url.substr(pos + 1));
	if (port == 0) {
		Log("Invalid URL");
		return false;
	}

	auto worker = std::make_unique<std::conditional_t<bUDP, xUDPWorkerWnd, xWorkerWnd>>(this, ip, port);
	worker->Show(true);
	worker->SetFocus();
	worker.release();

	return true;
}

void xMainWnd::OnConnectTo(wxCommandEvent& event) {
	ConnectTo<false>(ui_textIP->GetValue());
}

void xMainWnd::OnConnectToTS(wxCommandEvent& event) {
	ConnectTo<false>(ui_textIP_TS->GetValue());
}

void xMainWnd::OnConnectToUDP(wxCommandEvent& event) {
	ConnectTo<true>(ui_textIP_UDP->GetValue());
}
