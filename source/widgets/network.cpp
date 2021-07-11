#include "network.h"
#include <QTcpServer>
#include <game/commands.h>
#include <game/game.h>
#include <game/history.h>

Network::Network(QObject* parent, Chess::Game* game, Chess::History* history)
	: QObject(parent)
    , game_(game)
    , history_(history)
{

}

void Network::HostGame()
{
	if (!server_)
	{
		server_ = new QTcpServer(this);
		connect(server_, &QTcpServer::newConnection, this, &Network::OnNewConnection);
		server_->listen(QHostAddress::Any, port);
	}
}

void Network::ConnectToGame()
{
	if (!socket_)
	{
		socket_ = new QTcpSocket(this);
		connect(socket_, &QTcpSocket::connected, this, &Network::OnConnected);
        connect(socket_, &QTcpSocket::readyRead, this, &Network::OnNetworkRead);
        connect(socket_, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred), this, &Network::OnNetworkError);
	}

    socket_->connectToHost("localhost", port, QIODevice::ReadWrite,  QAbstractSocket::IPv4Protocol);
	socket_->waitForConnected(1000);
}

void Network::Automatch()
{
    if (!socket_)
    {
        socket_ = new QTcpSocket(this);
        connect(socket_, &QTcpSocket::connected, this, &Network::OnConnected);
        connect(socket_, &QTcpSocket::readyRead, this, &Network::OnNetworkRead);
        connect(socket_, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred), this, &Network::OnNetworkError);
    }

    //socket_->connectToHost("23.254.229.241", port, QIODevice::ReadWrite,  QAbstractSocket::IPv4Protocol);
    socket_->connectToHost("127.0.0.1", port, QIODevice::ReadWrite,  QAbstractSocket::IPv4Protocol);
    socket_->waitForConnected(1000);
}

void Network::OnNewConnection()
{
	socket_ = server_->nextPendingConnection();
	connect(socket_, &QTcpSocket::readyRead, this, &Network::OnNetworkRead);
    connect(socket_, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred), this, &Network::OnNetworkError);
}

void Network::OnConnected()
{

}

void Network::OnNetworkRead()
{
    auto buffer = socket_->readAll();
    ibytestream stream(std::vector<char>(buffer.begin(), buffer.end()));
    while (auto command = Chess::ICommand::Deserialize(stream))
    {
        command->MarkFromReplication();
        history_->Execute(*game_, command);
    }
}

void Network::OnNetworkError(QAbstractSocket::SocketError error)
{
	NetworkError(error);
}

void Network::OnCommandExecuted(Chess::ICommand& command)
{
    if (command.IsFromReplication())
        return;

    obytestream stream;
    Chess::ICommand::Serialize(stream, &command);
    if (socket_)
        socket_->write(stream.GetBuffer().data(), stream.GetBuffer().size());
}
