#include "pch.h"
#include "wxDesktopApp.h"
#include "MainWnd.h"
#include "gtl/wx/util.h"

#include "Server.h"

namespace asio = boost::asio;

// Log (fmt::format)
template < typename ... TArgs >
constexpr void Log(fmt::format_string<TArgs...> fmt, TArgs&& ... args) {
	auto msg = fmt::format(fmt, std::forward<TArgs&&>(args)...);
	wxLogInfo("%s", msg);
}
template < typename ... TArgs >
constexpr void Log(fmt::wformat_string<TArgs...> fmt, TArgs&& ... args) {
	auto msg = fmt::format(fmt, std::forward<TArgs&&>(args)...);
	wxLogInfo(L"%s", msg);
}

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
			m_io = std::make_shared<asio::io_service>();
			m_work = std::make_shared<asio::io_service::work>(*m_io);
			m_strand = std::make_shared<asio::io_service::strand>(*m_io);

			{
				std::list<std::shared_ptr<xServer>> servers;
				servers.push_back(std::make_shared<xServer>(*m_io, *m_strand, port));

				Log("Listener thread started");
				while (!st.stop_requested()) {
					m_io->poll();
					Log("Listener thread running");
					std::this_thread::sleep_for(1000ms);
				}
				Log("Listener thread stopped");
			}

			m_strand.reset();
			m_work.reset();
			m_io.reset();
			m_bListenerStopped = true;
		});

	} else {
		// stop listening
		if (!m_listener)
			return;
		m_listener->request_stop();
	}

}

void xMainWnd::OnConnectTo(wxCommandEvent& event) {
}
