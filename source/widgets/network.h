#pragma once
#include <game/iobserver.h>
#include <QObject>
#include <QTcpSocket>

class QTcpServer;
namespace Chess
{
    class Game;
    class History;
}

class Network : public QObject, public Chess::IObserver
{
    Q_OBJECT
public:

    Network(QObject* parent, Chess::Game* game, Chess::History* history);

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

    Chess::Game* game_ = nullptr;
    Chess::History* history_ = nullptr;
};
