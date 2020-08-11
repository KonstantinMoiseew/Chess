#include "network.h"
#include <QTcpServer>

Network::Network(QObject* parent)
	: QObject(parent)
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
		connect(socket_, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Network::OnNetworkError);
	}

	socket_->connectToHost("localhost", port, QIODevice::ReadWrite,  QAbstractSocket::IPv4Protocol);
	socket_->waitForConnected(1000);
}

void Network::OnNewConnection()
{
	socket_ = server_->nextPendingConnection();
	connect(socket_, &QTcpSocket::readyRead, this, &Network::OnNetworkRead);
	connect(socket_, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Network::OnNetworkError);
}

void Network::OnConnected()
{
	socket_->write("Hello world");
}

void Network::OnNetworkRead()
{

}

void Network::OnNetworkError(QAbstractSocket::SocketError error)
{
	NetworkError(error);
}

void Network::OnCommandExecuted(Chess::ICommand& command)
{
	socket_->write()
}
