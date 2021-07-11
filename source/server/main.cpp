#include <iostream>
#include <thread>
#include <vector>
#include "include/asio.hpp"

using protocol = asio::ip::tcp;

const short PORT = 15678;

struct Client
{
	Client(asio::io_service& service)
		: socket(new protocol::socket(service))
		, buffer(1024)
	{
	}

	protocol::socket* GetSocket() const
	{
		return socket.get();
	}

	std::unique_ptr<protocol::socket> socket;
	std::vector<char> buffer;
	int id = 0;
};


class Server
{
public:

	Server()
		: acceptor(io_service, protocol::endpoint(protocol::v4(), PORT))
	{
	}


	void Run()
	{
		Accept();

		try
		{
			io_service.run();
		}
		catch (std::exception& e)
		{
			std::cout << "Exception in IO service: " << e.what() << std::endl;
		}
	}

protected:

	void Accept()
	{
		auto client = new Client(io_service);

		acceptor.async_accept(*client->GetSocket(), 
			[this, client](asio::error_code ec)
		{
			if (ec)
			{
				std::cout << "Error while accepting: " << ec.message() << std::endl;
				delete client;

				Accept();
				return;
			}


			int player_id = clients.size();
			client->id = player_id;
			std::cout << "Player#" << player_id << " connected" << std::endl;
			std::lock_guard<std::mutex> g(clients_mutex);
			{
				clients.push_back(client);
			}

			Read(client);
			Accept();
		});
	}

	void Read(Client* client)
	{
		std::function<void(asio::error_code, size_t)> on_read = [this, client](asio::error_code ec, size_t bytes_transferred)
		{
			if (ec)
			{
				std::cout << "Error while reading: " << ec.message() << std::endl;
				return;
			}

			std::cout << "Read " << bytes_transferred << " bytes from player#" << client->id << std::endl;

			std::lock_guard<std::mutex> g(clients_mutex);
			{
				int opponent_player_id = client->id;
				if (client->id % 2 == 0)
					opponent_player_id++;
				else
					opponent_player_id--;

				if (opponent_player_id >= clients.size())
				{
					std::cout << "Error - no player with id = " << opponent_player_id  << std::endl;
					return;
				}

				Write(client, clients[opponent_player_id], bytes_transferred);
			}
		};

		asio::async_read(*client->GetSocket(), asio::buffer(client->buffer), asio::transfer_at_least(1), on_read);
	}

	void Write(Client* from, Client* to, size_t bytes_transferred)
	{
		asio::async_write(*to->GetSocket(), asio::buffer(from->buffer, bytes_transferred), 
			[this, from, to](asio::error_code ec, size_t bytes_transferred)
		{
			if (ec)
			{
				std::cout << "Error while writing: " << ec.message() << std::endl;
				return;
			}

			std::cout << "Wrote " << bytes_transferred << " bytes to player#" << to->id << std::endl;
			Read(from);
		});
	}

	asio::io_service io_service;
	protocol::acceptor acceptor;

	std::vector<Client*> clients;
	std::mutex clients_mutex;
};


int main()
{
	Server server;
	server.Run();
	return 0;
}