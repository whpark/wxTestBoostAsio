#pragma once

//=================================================================================================================================
/// @brief 

class xEchoServerTS {
public:
	using this_t = xEchoServerTS;

	asio::io_context m_io_context;
	std::shared_mutex m_mtxSockets;
	std::set<asio::ip::tcp::socket*> m_sockets;

	std::optional<std::jthread> m_worker;

public:
	xEchoServerTS(int n_thread_hint = 4) : m_io_context(n_thread_hint) {
		m_io_context.stop();
	}
	~xEchoServerTS() {
		Stop();
	}
	xEchoServerTS(xEchoServerTS const&) = delete;
	xEchoServerTS(xEchoServerTS &&) = delete;
	xEchoServerTS const& operator = (xEchoServerTS const&) = delete;
	xEchoServerTS& operator = (xEchoServerTS &&) = delete;

	bool Start(int port) {
		if (m_worker)
			return false;
		if (m_io_context.stopped())
			m_io_context.restart();
		asio::co_spawn(m_io_context, Listener(port), asio::detached);
		m_worker.emplace([this] { try { m_io_context.run(); } catch (...) { Log("EchoServerTS FAILED"); } });
		return true;
	}
	void Stop() {
		if (!m_worker)
			return;

		// shutdown all sockets
		{
			std::unique_lock lock(m_mtxSockets);
			for (auto* socket : m_sockets) {
				socket->shutdown(socket->shutdown_both);
			}
		}

		m_io_context.stop();
		m_worker->request_stop();
		m_worker->join();
		m_worker.reset();
	}

protected:
	asio::awaitable<void> Echo(asio::ip::tcp::socket socket) {
		Log("<<< Echo - thread {}", std::this_thread::get_id());
		// enter
		{
			std::unique_lock lock(m_mtxSockets);
			m_sockets.emplace(&socket);
		}

		try {
			std::array<char, 10240> data;	// 10 kbytes : jumbo packet (9014)
			while (true) {
				auto len = co_await socket.async_read_some(asio::buffer(data), asio::use_awaitable);
				if (len <= 0)
					break;
				co_await asio::async_write(socket, asio::buffer(data, len), asio::use_awaitable);
			}
		} catch(std::exception& e) {
			Log("ERROR! Echo TS exception : {}", e.what());
		}

		// exit
		{
			std::unique_lock lock(m_mtxSockets);
			m_sockets.erase(&socket);
		}
		Log(">>> Echo - thread {} EXIT", std::this_thread::get_id());
	}

	asio::awaitable<void> Listener(int port) {
		Log("ListenerTS Started");
		auto executor = co_await asio::this_coro::executor;
		asio::ip::tcp::acceptor acceptor(executor, asio::ip::tcp::endpoint{asio::ip::tcp::v4(), (uint_least16_t)port});
		while(true) {
			asio::ip::tcp::socket socket = co_await acceptor.async_accept(asio::use_awaitable);
			asio::co_spawn(executor, Echo(std::move(socket)), asio::detached);
		}
		Log("ListenerTS Stopped");
	}

};
