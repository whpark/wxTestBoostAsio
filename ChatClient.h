#pragma once

class xChatClient {
public:
	using this_t = xChatClient;

protected:
	wxWindow* m_pOwner{};
	asio::io_context& m_io_context;
	asio::ip::tcp::socket m_socket;
	asio::ip::tcp::resolver::results_type m_endpoints;
	xChatMessage m_msg_read;
	xQueueChatMessage m_msgs_to_write;

	std::mutex m_mtx_read_msg;
	xQueueChatMessage m_msgs_read;

	std::optional<std::jthread> m_worker;

public:
	xChatClient(wxWindow* pOwner, asio::io_context& io_context, asio::ip::tcp::resolver::results_type const& endpoints)
		: m_pOwner(pOwner), m_io_context(io_context), m_socket(io_context), m_endpoints(endpoints)
	{
		DoConnect(m_endpoints);
	}
	xChatClient(wxWindow* pOwner, asio::io_context& io_context, std::string const& ip, int port)
		: m_pOwner(pOwner), m_io_context(io_context), m_socket(io_context)
	{
		asio::ip::tcp::resolver resolver(io_context);
		auto endpoints = resolver.resolve(ip, std::to_string(port));
		DoConnect(endpoints);
	}

	void Write(xChatMessage msg) {
		if (!msg.ends_with('\n'))
			msg += "\n";
		asio::post(m_io_context, [this, str = std::move(msg)]{
			bool bWriting = !m_msgs_to_write.empty();
			m_msgs_to_write.push_back(std::move(str));
			if (!bWriting)
				DoWrite();
		});
	}

	void Close() {
		asio::post(m_io_context, [this]{ m_socket.shutdown(m_socket.shutdown_both); m_socket.close(); });
	}

	std::optional<xChatMessage> PopMessage() {
		std::unique_lock lock(m_mtx_read_msg);
		if (m_msgs_read.empty())
			return {};
		auto msg = std::move(m_msgs_read.front());
		m_msgs_read.pop_front();
		return msg;
	}

protected:
	void DoConnect(asio::ip::tcp::resolver::results_type const& endpoints) {
		asio::async_connect(m_socket, endpoints,
			[this](std::error_code ec, asio::ip::tcp::endpoint) {
				if (!ec) {
					wxQueueEvent(m_pOwner, new xEvtIPComm(m_pOwner->GetId(), wxEVT_IP_COMM, xEvtIPComm::EVT_CONNECTED, "Connected"));

					DoReadLine();
				} else {
					wxQueueEvent(m_pOwner, new xEvtIPComm(m_pOwner->GetId(), wxEVT_IP_COMM, xEvtIPComm::EVT_NOT_CONNECTED, "NOT Connected"));
				}
			});

		m_worker.emplace([this]
			{
				//while (!stop.stop_requested() and !m_io_context.stopped())
				//	m_io_context.run_one();
				m_io_context.run();
				Log("Exit - DoConnect()");
			}
		);
	}

	void DoReadLine() {
		asio::async_read_until(m_socket, asio::dynamic_buffer(m_msg_read), '\n',
			[this](std::error_code ec, std::size_t length) {
				if (!ec) {
					auto msg = m_msg_read.substr(0, length);
					m_msg_read.erase(0, length);
					{
						std::unique_lock lock(m_mtx_read_msg);
						m_msgs_read.emplace_back(msg);
					}
					// ToDo: notify msg
					auto* p = new xEvtIPComm(m_pOwner->GetId(), wxEVT_IP_COMM, xEvtIPComm::EVT_MESSAGE, msg);
					p->SetEventObject(m_pOwner);
					wxQueueEvent(m_pOwner, p);

					DoReadLine();
				}
				else {
					m_socket.shutdown(m_socket.shutdown_both); 
					m_socket.close();
					wxQueueEvent(m_pOwner, new xEvtIPComm(m_pOwner->GetId(), wxEVT_IP_COMM, xEvtIPComm::EVT_DISCONNECTED, "Disconnected"));
				}
			});
	}

	void DoWrite() {
		asio::async_write(m_socket, asio::buffer(m_msgs_to_write.front().data(), m_msgs_to_write.front().size()),
			[this](std::error_code ec, std::size_t length) {
				if (!ec) {
					m_msgs_to_write.pop_front();
					if (!m_msgs_to_write.empty())
						DoWrite();
				}
				else {
					m_socket.shutdown(m_socket.shutdown_both); 
					m_socket.close();
					wxQueueEvent(m_pOwner, new xEvtIPComm(m_pOwner->GetId(), wxEVT_IP_COMM, xEvtIPComm::EVT_DISCONNECTED, "Disconnected"));
				}
			});
	}
};
