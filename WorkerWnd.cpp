#include "pch.h"
#include "common.h"
#include "WorkerWnd.h"

xWorkerWnd::xWorkerWnd(wxWindow* parent, std::string ip, int port) : ui::IWorkerWnd(parent) {
	Bind(wxEVT_IP_COMM, &xWorkerWnd::OnEvtIPComm, this);
	//this->Connect(wxEVT_IP_COMM, &xWorkerWnd::OnEvtIPComm, nullptr, this);
	m_client.emplace(this, m_io_context, ip, port);
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

	wxPostEvent(this, xEvtIPComm(wxEVT_IP_COMM, xEvtIPComm::EVT_MESSAGE, "msg"));
	wxQueueEvent(this, new xEvtIPComm(wxEVT_IP_COMM, xEvtIPComm::EVT_MESSAGE, "msg"));
}

void xWorkerWnd::OnEvtIPComm(xEvtIPComm& event) {
	switch (event.m_evt) {
	case xEvtIPComm::EVT_CONNECTED:
		Log("Connected");
		break;
	case xEvtIPComm::EVT_NOT_CONNECTED:
		Log("NOT Connected");
		break;
	case xEvtIPComm::EVT_DISCONNECTED:
		Log("Disconnected");
		break;
	case xEvtIPComm::EVT_MESSAGE:
		ui_log->SetSelection(-1, -1);
		ui_log->Replace(-1, -1, event.m_msg);
		//ui_log->Replace(-1, -1, "\n");
		break;
	}
}
