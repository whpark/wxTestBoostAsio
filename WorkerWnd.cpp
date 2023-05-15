#include "pch.h"
#include "common.h"
#include "WorkerWnd.h"

xWorkerWnd::xWorkerWnd(wxWindow* parent, std::string ip, int port) : ui::IWorkerWnd(parent) {
	Bind(wxEVT_IP_COMM, &xWorkerWnd::OnEvtIPComm, this, GetId());
	m_client.emplace(this, m_io_context, ip, port);
}

xWorkerWnd::~xWorkerWnd() {
	m_io_context.stop();
	m_client.reset();
	Unbind(wxEVT_IP_COMM, &xWorkerWnd::OnEvtIPComm, this, GetId());
	Log("Client Closed");
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
	auto str = ui_textMessage->GetValue();
	xChatMessage msg = str.ToStdString();
	m_client->Write(msg);
	ui_log->AppendText("Sent: ");
	ui_log->AppendText(msg);
	ui_log->AppendText("\n");

}

void xWorkerWnd::OnButtonClick_Close(wxCommandEvent& event) {
	if constexpr (true) {
		wxQueueEvent(this, new xEvtIPComm(GetId(), wxEVT_IP_COMM, xEvtIPComm::EVT_MESSAGE, "msg"));
	}

	if constexpr (false) {
		wxCommandEvent evt(wxEVT_BUTTON, GetId());
		evt.SetEventObject(this);
		ProcessWindowEvent(evt);
	}
}

void xWorkerWnd::OnEvtIPComm(xEvtIPComm& event) {
	switch (event.m_evt) {
	case xEvtIPComm::EVT_CONNECTED:
		Log("Connected");
		break;
	case xEvtIPComm::EVT_NOT_CONNECTED:
		Log("NOT Connected");
		Close();
		break;
	case xEvtIPComm::EVT_DISCONNECTED:
		Log("Disconnected");
		Close();
		break;
	case xEvtIPComm::EVT_MESSAGE:
		ui_log->AppendText("Recv: ");
		ui_log->AppendText(event.m_msg);
		ui_log->AppendText("\n");
		auto r = ui_log->GetLastPosition();
		ui_log->ShowPosition(r);
		//ui_log->Replace(-1, -1, "\n");
		break;
	}
}
