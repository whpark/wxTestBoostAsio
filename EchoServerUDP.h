#pragma once

#include "common.h"

class xEchoServerUDP {
public:
	using this_t = xEchoServerUDP;

protected:
	asio::io_context m_io_context;
	std::optional<asio::ip::udp::socket> m_socket;

	std::optional<std::jthread> m_server;

public:

	xEchoServerUDP(int n_thread) : m_io_context(n_thread) {}
	~xEchoServerUDP() {
		Stop();
	}

	bool Start(int port) {
		if (m_server)
			return false;

		m_socket.emplace(m_io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port));

		m_server.emplace([this, port, &socket = *m_socket](std::stop_token st)
			{
				char data[10240];	// for jumbo packet (9014)
				while (!st.stop_requested() and socket.is_open()) {
					try {
						asio::ip::udp::endpoint sender;
						auto len = socket.receive_from(asio::buffer(data, sizeof(data)), sender);
						if (len <= 0)
							continue;
						socket.send_to(asio::buffer(data, len), sender);
					} catch (std::exception& e) {
						Log("Server UDP : {}", e.what());
					}
				}
			});
		return true;
	}

	bool Stop() {
		if (!m_server)
			return false;

		m_server->request_stop();

		if (m_socket and m_socket->is_open()) {
			m_socket->shutdown(m_socket->shutdown_both);
			m_socket->close();
		}
		m_io_context.stop();

		m_server->join();
		m_server.reset();
		m_socket.reset();

		return true;
	}

	bool IsRuning() const {
		return (bool)m_server;
	}

};
