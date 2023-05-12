///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-284-gf026a8e1)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "pch.h"

#include "wxUI.h"

///////////////////////////////////////////////////////////////////////////
using namespace ui;

IMainWnd::IMainWnd( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizerTOP;
	bSizerTOP = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizerConnection;
	sbSizerConnection = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Connection") ), wxVERTICAL );

	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 0, 2, 0, 0 );

	ui_chkListener = new wxCheckBox( sbSizerConnection->GetStaticBox(), wxID_ANY, _("Listen - Port:"), wxDefaultPosition, wxDefaultSize, wxCHK_2STATE );
	gSizer1->Add( ui_chkListener, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	ui_textPort = new wxTextCtrl( sbSizerConnection->GetStaticBox(), wxID_ANY, _("9000"), wxDefaultPosition, wxDefaultSize, wxTE_AUTO_URL|wxTE_RICH );
	gSizer1->Add( ui_textPort, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	ui_btnConnectTo = new wxButton( sbSizerConnection->GetStaticBox(), wxID_ANY, _("Connect To:"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1->Add( ui_btnConnectTo, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	ui_textIP = new wxTextCtrl( sbSizerConnection->GetStaticBox(), wxID_ANY, _("127.0.0.1:9000"), wxDefaultPosition, wxDefaultSize, wxTE_AUTO_URL|wxTE_RICH );
	gSizer1->Add( ui_textIP, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	sbSizerConnection->Add( gSizer1, 0, 0, 5 );


	bSizer14->Add( sbSizerConnection, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );


	bSizerTOP->Add( bSizer14, 1, wxALIGN_CENTER_VERTICAL, 5 );


	this->SetSizer( bSizerTOP );
	this->Layout();
	m_timerUpdateUI.SetOwner( this, wxTIMER_UPDATE_UI );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_MOVE, wxMoveEventHandler( IMainWnd::OnMove ) );
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( IMainWnd::OnSize ) );
	ui_chkListener->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( IMainWnd::OnChkListen ), NULL, this );
	ui_btnConnectTo->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IMainWnd::OnConnectTo ), NULL, this );
	this->Connect( wxTIMER_UPDATE_UI, wxEVT_TIMER, wxTimerEventHandler( IMainWnd::OnTimer_UpdateUI ) );
}

IMainWnd::~IMainWnd()
{
	// Disconnect Events
	this->Disconnect( wxEVT_MOVE, wxMoveEventHandler( IMainWnd::OnMove ) );
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( IMainWnd::OnSize ) );
	ui_chkListener->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( IMainWnd::OnChkListen ), NULL, this );
	ui_btnConnectTo->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IMainWnd::OnConnectTo ), NULL, this );
	this->Disconnect( wxTIMER_UPDATE_UI, wxEVT_TIMER, wxTimerEventHandler( IMainWnd::OnTimer_UpdateUI ) );

}

IWorkerWnd::IWorkerWnd( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizerTOP;
	bSizerTOP = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText4 = new wxStaticText( this, wxID_ANY, _("Peer :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	bSizer5->Add( m_staticText4, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	ui_textPeer = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	bSizer5->Add( ui_textPeer, 1, wxALL, 5 );

	m_btnClose = new wxButton( this, wxID_ANY, _("Close"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	bSizer5->Add( m_btnClose, 0, wxALL, 5 );


	bSizer7->Add( bSizer5, 0, wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizerMessage;
	sbSizerMessage = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("comm") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer2->AddGrowableCol( 1 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText2 = new wxStaticText( sbSizerMessage->GetStaticBox(), wxID_ANY, _("format text :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	fgSizer2->Add( m_staticText2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	ui_textMessage = new wxTextCtrl( sbSizerMessage->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizer2->Add( ui_textMessage, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

	ui_btnSend = new wxButton( sbSizerMessage->GetStaticBox(), wxID_ANY, _("Send"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	fgSizer2->Add( ui_btnSend, 0, wxALL, 5 );

	m_staticText21 = new wxStaticText( sbSizerMessage->GetStaticBox(), wxID_ANY, _("Parameters :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	fgSizer2->Add( m_staticText21, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textParameters = new wxTextCtrl( sbSizerMessage->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizer2->Add( m_textParameters, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );


	sbSizerMessage->Add( fgSizer2, 0, wxEXPAND, 5 );

	m_splitter1 = new wxSplitterWindow( sbSizerMessage->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter1->Connect( wxEVT_IDLE, wxIdleEventHandler( IWorkerWnd::m_splitter1OnIdle ), NULL, this );

	m_panel1 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );

	ui_log = new wxRichTextCtrl( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0|wxVSCROLL|wxHSCROLL|wxNO_BORDER|wxWANTS_CHARS );
	bSizer8->Add( ui_log, 1, wxEXPAND | wxALL, 5 );


	m_panel1->SetSizer( bSizer8 );
	m_panel1->Layout();
	bSizer8->Fit( m_panel1 );
	m_panel2 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer71;
	bSizer71 = new wxBoxSizer( wxVERTICAL );

	ui_logHex = new wxRichTextCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0|wxVSCROLL|wxHSCROLL|wxNO_BORDER|wxWANTS_CHARS );
	bSizer71->Add( ui_logHex, 1, wxEXPAND | wxALL, 5 );


	m_panel2->SetSizer( bSizer71 );
	m_panel2->Layout();
	bSizer71->Fit( m_panel2 );
	m_splitter1->SplitVertically( m_panel1, m_panel2, 0 );
	sbSizerMessage->Add( m_splitter1, 1, wxEXPAND, 5 );


	bSizer7->Add( sbSizerMessage, 1, wxEXPAND, 5 );


	bSizerTOP->Add( bSizer7, 1, wxEXPAND|wxALL, 5 );


	this->SetSizer( bSizerTOP );
	this->Layout();
	m_timerUpdateUI.SetOwner( this, wxTIMER_UPDATE_UI );
	m_timerUpdateUI.Start( 100 );


	this->Centre( wxBOTH );

	// Connect Events
	ui_textMessage->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( IWorkerWnd::OnTextEnter_Message ), NULL, this );
	ui_btnSend->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IWorkerWnd::OnButtonClick_Send ), NULL, this );
	m_textParameters->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( IWorkerWnd::OnTextEnter_Parameters ), NULL, this );
	this->Connect( wxTIMER_UPDATE_UI, wxEVT_TIMER, wxTimerEventHandler( IWorkerWnd::OnTimer_UpdateUI ) );
}

IWorkerWnd::~IWorkerWnd()
{
	// Disconnect Events
	ui_textMessage->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( IWorkerWnd::OnTextEnter_Message ), NULL, this );
	ui_btnSend->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IWorkerWnd::OnButtonClick_Send ), NULL, this );
	m_textParameters->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( IWorkerWnd::OnTextEnter_Parameters ), NULL, this );
	this->Disconnect( wxTIMER_UPDATE_UI, wxEVT_TIMER, wxTimerEventHandler( IWorkerWnd::OnTimer_UpdateUI ) );

}
