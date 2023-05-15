#include "pch.h"
#include "WorkerWnd.h"

xWorkerWnd::xWorkerWnd(wxWindow* parent, asio::io_context& io, asio::ip::tcp::resolver::results_type const& endpoints)
	: ui::IWorkerWnd(parent), m_client(io, endpoints) {
}

void xWorkerWnd::OnTimer_UpdateUI(wxTimerEvent& event) {
}

void xWorkerWnd::OnTextEnter_Message(wxCommandEvent& event) {
	OnButtonClick_Send(event);
}

void xWorkerWnd::OnTextEnter_Parameters(wxCommandEvent& event) {
	OnButtonClick_Send(event);
}

void xWorkerWnd::OnButtonClick_Send(wxCommandEvent& event) {

}
