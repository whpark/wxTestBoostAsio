#pragma once

#include "common.h"

//=================================================================================================================================
/// @brief
class session : public std::enable_shared_from_this<session> {
public:
	session(asio::ip::tcp::socket socket)
		: socket_(std::move(socket)) {
	}

	void start() {
		do_read();
	}

private:
	void do_read() {
		auto self(shared_from_this());
		socket_.async_read_some(asio::buffer(data_),
			asio::bind_allocator(
				handler_allocator<int>(handler_memory_),
				[this, self](boost::system::error_code ec, std::size_t length)
				{
					std::string_view sv(data_.data(), length);
					Log("Read({} bytes) : {}", length, sv);
					if (!ec) {
						do_write(length);
					}
				}));
	}

	void do_write(std::size_t length) {
		auto self(shared_from_this());
		asio::async_write(socket_, asio::buffer(data_, length),
			asio::bind_allocator(
				handler_allocator<int>(handler_memory_),
				[this, self](boost::system::error_code ec, std::size_t /*length*/)
				{
					if (!ec) {
						do_read();
					}
				}));
	}

	// The socket used to communicate with the client.
	asio::ip::tcp::socket socket_;

	// Buffer used to store data received from the client.
	std::array<char, 1024> data_;

	// The memory to use for handler-based custom memory allocation.
	handler_memory handler_memory_;
};

//=================================================================================================================================
/// @brief 
class xEchoServer {
protected:
	asio::io_context& io_context_;
	asio::ip::tcp::acceptor acceptor_;
public:
	xEchoServer(asio::io_context& io_context, short port)
		: io_context_(io_context), acceptor_(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {
		Accept();
	}

private:
	void Accept() {
		Log("ENTER acceptor thread{}", std::this_thread::get_id());
		acceptor_.async_accept(
			[this](boost::system::error_code ec, asio::ip::tcp::socket socket)
			{
				if (ec) {
					Log("Listener Failed");
					return;
				}

				Log("<<< session {}", std::this_thread::get_id());
				std::make_shared<session>(std::move(socket))->start();
				Log(">>> session {}", std::this_thread::get_id());

				Accept();
			}
		);
		Log("EXIT acceptor");
	}

};
