#pragma once

class xChatClient {
public:
	using this_t = xChatClient;

protected:
	asio::io_context& m_io_context;
	asio::ip::tcp::socket m_socket;
	xChatMessage m_msg_read;
	xQueueChatMessage m_msgs_to_write;

	std::mutex m_mtx_read_msg;
	xQueueChatMessage m_msgs_read;

public:
	xChatClient(asio::io_context& io_context, asio::ip::tcp::resolver::results_type const& endpoints)
		: m_io_context(io_context), m_socket(io_context)
	{
		DoConnect(endpoints);
	}

	void Write(xChatMessage const& msg) {
		asio::post(m_io_context, [this, msg]{
			bool bWriting = !m_msgs_to_write.empty();
			m_msgs_to_write.push_back(msg);
			if (!bWriting)
				DoWrite();
		});
	}

	void Close() {
		asio::post(m_io_context, [this]{ m_socket.close(); });
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
					DoReadLine();
				}
			});
	}

	void DoReadLine() {
		asio::async_read_until(m_socket, asio::dynamic_buffer(m_msg_read), '\n',
			[this](std::error_code ec, std::size_t length) {
				if (!ec) {
					auto msg = m_msg_read.substr(0, length);
					m_msg_read.erase(0, length);
					// ToDo: notify msg
					{
						std::unique_lock lock(m_mtx_read_msg);
						m_msgs_read.emplace_back(std::move(msg));
					}

					DoReadLine();
				}
				else {
					m_socket.close();
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
					m_socket.close();
				}
			});
	}
};
