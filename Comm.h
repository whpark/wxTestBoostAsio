#pragma once


class IWorker {
public:
	virtual ~IWorker() = default;
	virtual void OnMessage(std::string const& msg) = 0;
};

class xServer {
public:
	using this_t = xServer;

	boost::asio::io_service& m_io;
	boost::asio::io_service::strand& m_strand;
	boost::asio::ip::tcp::endpoint m_endpoint;

	boost::asio::ip::tcp::acceptor m_acceptor;

	xServer(boost::asio::io_service& io, boost::asio::io_service::strand& strand, int port)
		: m_io(io), m_strand(strand), m_endpoint(boost::asio::ip::tcp::v4(), port)
	{
	}

	void OnAccept(std::shared_ptr<>)
};

