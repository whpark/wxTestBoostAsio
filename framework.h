// header.h : include file for standard system include files,
// or project specific include files
//

#pragma once

#include <GL/glew.h>

#include "wx/wx.h"
#include "wx/dataview.h"
#include "wx/webview.h"
#include "wx/glcanvas.h"
#include "wx/stdpaths.h"
#include "wx/msw/registry.h"

#include "targetver.h"
//#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
//// Windows Header Files
//#include <windows.h>
//// C RunTime Header Files
//#include <stdlib.h>
//#include <malloc.h>
//#include <memory.h>
//#include <tchar.h>

#define BOOST_ASIO_NO_DEPRECATED
//#define BOOST_ASIO_NO_DEPRECATED_MSGS
#include "boost/asio.hpp"
#include "boost/bind/bind.hpp"

#include "gtl/gtl.h"
#include "gtl/reflection_glaze.h"

#include "spdlog/spdlog.h"
#include "spdlog/stopwatch.h"

using namespace std::literals;
using namespace gtl::literals;
using namespace nlohmann::literals;
using json = nlohmann::ordered_json;

namespace stdfs = std::filesystem;
namespace stdc = std::chrono;
namespace stdr = std::ranges;
namespace stdv = std::views;

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
