#pragma once

//=================================================================================================================================
/// @brief 
template < typename TProtocol = boost::asio::ip::tcp >
class TChatClient {
public:
	using this_t = TChatClient<TProtocol>;
	using protocol_t = TProtocol;

protected:
	wxWindow* m_pOwner{};
	asio::io_context m_io_context;
	protocol_t::socket m_socket;
	std::string m_ip;
	int m_port{};
	//protocol_t::resolver::results_type m_endpoints;

	xChatMessage m_msg_read;
	xQueueChatMessage m_msgs_to_write;

	std::mutex m_mtx_read_msg;
	xQueueChatMessage m_msgs_read;

public:
	TChatClient(wxWindow* pOwner, std::string const& ip, int port)
		: m_pOwner(pOwner), m_socket(m_io_context), m_ip(ip), m_port(port)
	{
		//protocol_t::resolver resolver(m_io_context);
		//m_endpoints = resolver.resolve(ip, std::to_string(port));
	}

	std::optional<xChatMessage> PopMessage() {
		std::unique_lock lock(m_mtx_read_msg);
		if (m_msgs_read.empty())
			return {};
		auto msg = std::move(m_msgs_read.front());
		m_msgs_read.pop_front();
		return msg;
	}

	bool Notify(xEvtIPComm::EVENT evt, std::string const& msg) {
		if (!m_pOwner)
			return false;
		wxQueueEvent(m_pOwner, new xEvtIPComm(m_pOwner->GetId(), wxEVT_IP_COMM, evt, msg));
		return true;
	}

	virtual bool Start() { return true; }
	virtual void Write(xChatMessage msg) {}
	virtual void Stop() {}
};


//=================================================================================================================================
/// @brief 
class xChatClient : public TChatClient<boost::asio::ip::tcp> {
public:
	using this_t = xChatClient;
	using base_t = TChatClient<boost::asio::ip::tcp>;

protected:
	std::optional<std::jthread> m_worker;

public:
	using base_t::base_t;

	bool Start() override {
		if (m_worker)
			return false;
		protocol_t::resolver resolver(m_io_context);
		auto endpoint = resolver.resolve(m_ip, std::to_string(m_port));
		asio::async_connect(m_socket, endpoint,
			[this](std::error_code ec, asio::ip::tcp::endpoint) {
				if (!ec) {
					Notify(xEvtIPComm::EVT_CONNECTED, "Connected");

					DoReadLine();
				} else {
					Notify(xEvtIPComm::EVT_NOT_CONNECTED, "NOT Connected");
				}
			});

		m_worker.emplace([this]
			{
				try {
					m_io_context.run();
				} catch (...) {
					Log("xChatClient::run() stopped");
				}
				Notify(xEvtIPComm::EVT_DISCONNECTED, "Disconnected");
			}
		);
	};
	void Write(xChatMessage msg) override {
		if (msg.empty())
			return;
		if (!msg.ends_with('\n'))
			msg += "\n";
		asio::post(m_io_context, [this, str = std::move(msg)]{
			bool bWriting = !m_msgs_to_write.empty();
			m_msgs_to_write.push_back(std::move(str));
			if (!bWriting)
				DoWrite();
		});
	}

	void Stop() override {
		if (!m_worker)
			return;
		asio::post(m_io_context, [this]{ m_socket.shutdown(m_socket.shutdown_both); m_socket.close(); m_io_context.stop(); });
		m_worker->join();
		m_worker.reset();
	}

protected:
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
					Notify(xEvtIPComm::EVT_MESSAGE, msg);

					DoReadLine();
				}
				else {
					m_socket.shutdown(m_socket.shutdown_both); 
					m_socket.close();
					Notify(xEvtIPComm::EVT_DISCONNECTED, "Disconnected");
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
					Notify(xEvtIPComm::EVT_DISCONNECTED, "Disconnected");
				}
			});
	}
};


//=================================================================================================================================
/// @brief 
class xChatClientUDP : public TChatClient<boost::asio::ip::udp> {
public:
	using this_t = xChatClientUDP;
	using base_t = TChatClient<boost::asio::ip::udp>;

	int m_portClient{0};
protected:
	std::optional<std::jthread> m_worker;

public:
	using base_t::base_t;

	bool Start() override {
		if (m_worker)
			return false;
		if (m_socket.is_open()) {
			m_socket.shutdown(m_socket.shutdown_both);
			m_socket.close();
		}
		if (m_io_context.stopped())
			m_io_context.restart();

		m_socket = asio::ip::udp::socket(m_io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), 0));
		m_worker.emplace([this]
			{
				char data[10240];
				while (m_socket.is_open()) {
					protocol_t::endpoint server_point;
					try {
						size_t len = m_socket.receive_from(asio::buffer(data, sizeof(data)), server_point);
						if (len > 0)
							Notify(xEvtIPComm::EVT_MESSAGE, std::string(data, len));
					} catch (std::exception& e) {
						Log("Client UDP : {}", e.what());
						if (!m_socket.is_open())
							break;
					}
				}
			}
		);
	};

	void Write(xChatMessage msg) {
		if (!m_socket.is_open() or m_io_context.stopped())
			return;

		if (!msg.ends_with('\n'))
			msg += "\n";

		protocol_t::resolver resolver(m_io_context);
		auto endpoints = resolver.resolve(m_ip, std::to_string(m_port));
		m_socket.send_to(asio::buffer(msg.data(), msg.size()), endpoints.begin()->endpoint());
	}

	void Stop() override {
		if (!m_worker)
			return;
		if (m_socket.is_open()) {
			m_socket.shutdown(m_socket.shutdown_both);
			m_socket.close();
		}
		m_io_context.stop();
		m_worker->join();
		m_worker.reset();
	}

};
