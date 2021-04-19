#include <iostream>
#include <thread>
#include <vector>
#include "include/asio.hpp"

using protocol = asio::ip::tcp;

const short PORT = 7777;

struct Game
{
	Game(protocol::socket* p1, protocol::socket* p2)
	{
		players[0] = p1;
		players[1] = p2;
	}

	~Game()
	{
		delete players[0];
		delete players[1];
	}

	protocol::socket* players[2];
};

int main()
{
	asio::io_context context;

	std::vector<protocol::socket*> new_clients;
	std::mutex new_clients_mutex;

	std::thread accept_thread([&]()
	{
		try
		{
			protocol::acceptor acceptor(context, protocol::endpoint(protocol::v4(), PORT));
			while (true)
			{
				protocol::socket* new_socket = new protocol::socket(context);
				acceptor.accept(*new_socket);

				std::lock_guard<std::mutex> g(new_clients_mutex);
				new_clients.push_back(new_socket);
			}
		}
		catch (std::exception& e)
		{
			std::cerr << "Accept thread fail: " << e.what() << std::endl;
		}
	});

	std::vector<Game*> games;

	while (true)
	{
		{
			std::lock_guard<std::mutex> g(new_clients_mutex);
			int j = 0;
			for (int i = 0; i < (int)new_clients.size(); i++)
			{
				if (i % 2)
				{
					j = i;
					games.push_back(new Game(new_clients[i], new_clients[i - 1]));
				}
			}
			new_clients.erase(new_clients.begin(), new_clients.begin() + j);
		}

		for (auto game : games)
		{
			for (int i = 0; i < 2; i++)
			{
				std::vector<char> data;
				asio::read(*game->players[i], asio::buffer(data), [](const asio::error_code& error, std::size_t bytes_transferred)
				{
					if (error)
						return (size_t)0;

					return bytes_transferred;
				});
				if (!data.empty())
				{
					asio::write(*game->players[!i], asio::buffer(data));
				}
			}
		}
	}
	
	accept_thread.join();
	return 0;
}