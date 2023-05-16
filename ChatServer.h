#pragma once

//=================================================================================================================================

class IWorker {
public:
	virtual ~IWorker() {}
	virtual void OnMessage(std::string const& msg) = 0;
};

using PWorker = std::shared_ptr<IWorker>;

class xChatRoom {
public:
	void Join(PWorker worker) {
		m_workers.insert(worker);
		for (auto msg : m_msgs)
			worker->OnMessage(msg);
	}

	void Leave(PWorker worker) {
		m_workers.erase(worker);
	}

	void OnMessage(const xChatMessage& msg) {
		while (m_msgs.size() > max_recent_msgs)
			m_msgs.pop_front();
		m_msgs.push_back(msg);

		Log("Received: {}", msg);

		for (auto worker : m_workers)
			worker->OnMessage(msg);
	}

private:
	std::set<PWorker> m_workers;
	enum { max_recent_msgs = 100 };
	xQueueChatMessage m_msgs;
};

//----------------------------------------------------------------------

class xChatSession : public IWorker, public std::enable_shared_from_this<xChatSession> {
public:
	asio::ip::tcp::socket m_socket;
	xChatRoom& m_room;
	xChatMessage m_msg_read;
	xQueueChatMessage m_msgs_to_write;

public:
	xChatSession(asio::ip::tcp::socket socket, xChatRoom& room)
		: m_socket(std::move(socket)), m_room(room)
	{ }

	void Start() {
		m_room.Join(shared_from_this());
		DoReadLine();
	}

	void OnMessage(std::string const& msg) override {
		bool write_in_progress = !m_msgs_to_write.empty();
		m_msgs_to_write.push_back(msg);
		if (!write_in_progress) {
			DoWrite();
		}
	}

protected:
	void DoReadLine() {
		auto self{shared_from_this()};
		asio::async_read_until(m_socket, asio::dynamic_buffer(m_msg_read), '\n',
			[this](std::error_code ec, std::size_t length) {
				if (!ec) {
					auto msg = m_msg_read.substr(0, length);
					m_msg_read.erase(0, length);
					// ToDo: notify msg
					//{
						m_room.OnMessage(std::move(msg));
					//}

					DoReadLine();
				}
				else {
					m_room.Leave(shared_from_this());
				}
			});
	}

	void DoWrite() {
		auto self(shared_from_this());
		asio::async_write(m_socket,
			boost::asio::buffer(m_msgs_to_write.front().data(), m_msgs_to_write.front().size()),
			[this, self](boost::system::error_code ec, std::size_t /*length*/)
			{
				if (!ec) {
					m_msgs_to_write.pop_front();
					if (!m_msgs_to_write.empty()) {
						DoWrite();
					}
				}
				else {
					m_room.Leave(shared_from_this());
				}
			});
	}

};

//----------------------------------------------------------------------

//=================================================================================================================================
/// @brief 
class xChatServer {
protected:
	asio::io_context& m_io_context;
	xChatRoom m_room;
	std::optional<asio::ip::tcp::acceptor> m_acceptor;
public:
	xChatServer(asio::io_context& io_context) : m_io_context(io_context) {
	}

	bool Start(short port) try {
		if (m_acceptor)
			return false;
		m_acceptor.emplace(m_io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));
		Accept();
		return true;
	} catch (std::exception& e) {
		Log("Exception: {}", e.what());
		return false;
	} catch (...) {
		Log("Exception: unknown");
		return false;
	}

private:
	void Accept() {
		if (!m_acceptor)
			return;
		Log("ENTER acceptor thread{}", std::this_thread::get_id());
		m_acceptor->async_accept(
			[this](boost::system::error_code ec, asio::ip::tcp::socket socket)
			{
				if (ec)
					return;

				Log("<<< session {}", std::this_thread::get_id());
				std::make_shared<xChatSession>(std::move(socket), m_room)->Start();
				Log(">>> session {}", std::this_thread::get_id());

				Accept();
			}
		);
		Log("EXIT acceptor");
	}
};
