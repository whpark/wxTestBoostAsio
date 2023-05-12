#include "pch.h"

// wxDesktopApp.cpp : Defines the entry point for the application.
//

#include "wxDesktopApp.h"

wxIMPLEMENT_APP(wxDesktopApp);

template <typename ... TArgs>
void Log(fmt::format_string<TArgs...> fmt, TArgs&& ... args) {
	wxLogInfo("%s", fmt::format(fmt, std::forward<TArgs>(args)...));
	//wxGetApp().m_wndLog->LogText("\n");
}

void Print(int* count, boost::asio::steady_timer* t) {
	Log("count : {}, at{}", ++*count, std::this_thread::get_id());
	if (*count < 5) {
		t->expires_at(t->expiry()+1s);
		t->async_wait([=](boost::system::error_code const&) {
			Print(count, t);
		});
	}
}

void Test() {
	Log("start at{}", std::this_thread::get_id());

	boost::asio::io_context io;
	boost::asio::steady_timer t(io, 1s);

	int count{};
	//std::jthread tt([&] {
		t.async_wait([&] (boost::system::error_code const&) {
			Print(&count, &t);
		});
	//});
	//std::this_thread::sleep_for(100ms);
	//io.run();
	while (!io.stopped()) {
		while (io.poll())
			;
		//if (t.expiry() < std::chrono::steady_clock::now())
		//	break;
	}

	Log("after run");

	//tt.join();
}

bool wxDesktopApp::OnInit() {
	m_reg.Create();
#ifdef _DEBUG
	m_wndLog = std::make_unique<wxLogWindow>(m_wndMain.get(), wxString(L"Log"));
	m_wndLog->Show(true);
	m_wndLog->PassMessages(false);
	auto s = wxLog::GetTimestamp();
	s += ".%l";
	wxLog::SetTimestamp(s);
#else
	wxLog::EnableLogging(false);
#endif

	Test();

	// Startup Folder
	gtl::SetCurrentPath_ProjectFolder(stdfs::path(L"."));

	if (!base_t::OnInit())
		return false;

	std::filesystem::create_directories(m_paths.GetConfigDir().ToStdWstring());

	m_pathSettings = m_paths.GetConfigDir().ToStdWstring();
	m_pathSettings /= u8"Settings.json";
	LoadSettings();

	m_wndMain = std::make_unique<xMainWnd>(nullptr);
	m_wndMain->Show();
	m_wndMain->SetFocus();

	return true;
}

int wxDesktopApp::OnRun() {
	return base_t::OnRun();
}

bool wxDesktopApp::LoadSettings() try {
	json j;
	if (stdfs::exists(m_pathSettings)) {
		std::ifstream f(m_pathSettings, std::ios_base::binary);
		f >> j;
	}
	else {
		j = R"xxx(
{
}
)xxx"_json;
		std::ofstream f(m_pathSettings, std::ios_base::binary);
		f << std::setw(4) << j;
	}

	//from_json(j, m_settings);

	return true;
} catch (...) {
	wxMessageBox(L"CANNOT Load settings.json", L"ERROR");
	return false;
}

bool wxDesktopApp::SaveSettings() try {
	json j;

	//to_json(j, m_settings);

	std::ofstream f(m_pathSettings, std::ios_base::binary);
	f << std::setw(4) << j;
	return true;
} catch (...) {
	wxMessageBox(L"CANNOT Save settings.json", L"ERROR");
	return false;
}
