///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-284-gf026a8e1)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/checkbox.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/timer.h>
#include <wx/frame.h>
#include <wx/stattext.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/panel.h>
#include <wx/splitter.h>

///////////////////////////////////////////////////////////////////////////

namespace ui
{
	///////////////////////////////////////////////////////////////////////////////
	/// Class IMainWnd
	///////////////////////////////////////////////////////////////////////////////
	class IMainWnd : public wxFrame
	{
		private:

		protected:
			enum
			{
				wxTIMER_UPDATE_UI = 1000
			};

			wxCheckBox* ui_chkListener;
			wxTextCtrl* ui_textPort;
			wxCheckBox* ui_chkListenerTS;
			wxTextCtrl* ui_textPortTS;
			wxButton* ui_btnConnectTo;
			wxTextCtrl* ui_textIP;
			wxTextCtrl* ui_textStatus;
			wxTimer m_timerUpdateUI;

			// Virtual event handlers, override them in your derived class
			virtual void OnMove( wxMoveEvent& event ) = 0;
			virtual void OnSize( wxSizeEvent& event ) = 0;
			virtual void OnChkListen( wxCommandEvent& event ) = 0;
			virtual void OnChkListenTS( wxCommandEvent& event ) = 0;
			virtual void OnConnectTo( wxCommandEvent& event ) = 0;
			virtual void OnTimer_UpdateUI( wxTimerEvent& event ) = 0;


		public:

			IMainWnd( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("TCP/IP Tester"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 296,313 ), long style = wxDEFAULT_FRAME_STYLE|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxTAB_TRAVERSAL );

			~IMainWnd();

	};

	///////////////////////////////////////////////////////////////////////////////
	/// Class IWorkerWnd
	///////////////////////////////////////////////////////////////////////////////
	class IWorkerWnd : public wxFrame
	{
		private:

		protected:
			enum
			{
				wxTIMER_UPDATE_UI = 1000
			};

			wxStaticText* m_staticText4;
			wxTextCtrl* ui_textPeer;
			wxButton* ui_btnClose;
			wxStaticText* m_staticText2;
			wxTextCtrl* ui_textMessage;
			wxButton* ui_btnSend;
			wxStaticText* m_staticText21;
			wxTextCtrl* m_textParameters;
			wxSplitterWindow* m_splitter1;
			wxPanel* m_panel1;
			wxRichTextCtrl* ui_log;
			wxPanel* m_panel2;
			wxRichTextCtrl* ui_logHex;
			wxTimer m_timerUpdateUI;

			// Virtual event handlers, override them in your derived class
			virtual void OnButtonClick_Close( wxCommandEvent& event ) = 0;
			virtual void OnTextEnter_Message( wxCommandEvent& event ) = 0;
			virtual void OnButtonClick_Send( wxCommandEvent& event ) = 0;
			virtual void OnTextEnter_Parameters( wxCommandEvent& event ) = 0;
			virtual void OnTimer_UpdateUI( wxTimerEvent& event ) = 0;


		public:

			IWorkerWnd( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Worker"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 859,516 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

			~IWorkerWnd();

			void m_splitter1OnIdle( wxIdleEvent& )
			{
				m_splitter1->SetSashPosition( 0 );
				m_splitter1->Disconnect( wxEVT_IDLE, wxIdleEventHandler( IWorkerWnd::m_splitter1OnIdle ), NULL, this );
			}

	};

} // namespace ui

