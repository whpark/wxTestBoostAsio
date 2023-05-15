#include "pch.h"
#include "wxDesktopApp.h"
#include "MainWnd.h"
#include "WorkerWnd.h"
#include "gtl/wx/util.h"


#include "Comm.h"

xMainWnd::xMainWnd( wxWindow* parent ) : ui::IMainWnd( parent ) {
	m_bInitialized = true;
	gtl::wx::LoadWindowPosition(wxGetApp().m_reg, "MainWindow"s, this);

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
	// Garbage Collection, listener
	if (m_listener and m_bListenerStopped and m_listener->joinable()) {
		m_listener.reset();
	}
}

void xMainWnd::OnChkListen(wxCommandEvent& event) {
	if (ui_chkListener->IsChecked()) {
		// start listening
		if (m_listener)
			return;

		int port = std::atoi(ui_textIP->GetValue().c_str());

		m_bListenerStopped = false;
		m_listener = std::jthread([port, this](std::stop_token st) {
			//m_io = std::make_shared<asio::io_context>();
			//m_strand = std::make_shared<asio::io_context::strand>(*m_io);

			//asio::io_context io_context;

			{
				xEchoServer server(m_io_context, port);

				Log("Listener thread started");
				//m_io_context.run();
				while (!st.stop_requested()) {
					m_io_context.poll_one();
					std::this_thread::yield();
				}
				//Log("Listener thread running");
				//std::this_thread::sleep_for(1000ms);
				Log("Listener thread stopped");
			}
			m_bListenerStopped = true;
		});

	} else {
		// stop listening
		if (!m_listener)
			return;
		m_io_context.stop();
		m_listener->request_stop();
	}

}

void xMainWnd::OnConnectTo(wxCommandEvent& event) {
	auto url = ui_textIP->GetValue();
	// split url into ip and port
	auto pos = url.find(':');
	if (pos == std::string::npos) {
		Log("Invalid URL");
		return;
	}
	std::string ip = url.substr(0, pos).ToStdString();
	auto port = atoi(url.substr(pos + 1));
	if (port == 0) {
		Log("Invalid URL");
		return;
	}

	auto worker = std::make_unique<xWorkerWnd>(this, ip, port);
	worker->Show(true);
	worker->SetFocus();
	worker.release();
}
