#pragma once
#include <game/iobserver.h>
#include <QObject>
#include <QTcpSocket>

class QTcpServer;

class Network : public QObject, Chess::IObserver
{
public:

	Network(QObject* parent);

	void HostGame();
	void ConnectToGame();

	void OnCommandExecuted(Chess::ICommand& command) override;

signals:

	void NetworkError(QAbstractSocket::SocketError error);

public slots:

	void OnNewConnection();
	void OnConnected();
	void OnNetworkRead();
	void OnNetworkError(QAbstractSocket::SocketError error);

protected:

	QTcpServer* server_ = nullptr;
	QTcpSocket* socket_ = nullptr;

	const quint16 port = 15678;
};
