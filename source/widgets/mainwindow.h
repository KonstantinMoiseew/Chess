#pragma once
#include <QMainWindow>
#include <QTcpSocket>
#include "game/iobserver.h"
#include "game/gamecommon.h"
#include "game/game.h"
#include "game/history.h"
#include "pieceitem.h"

extern bool g_DBG_Play_As_White;
extern bool g_DBG_Automatch;

namespace Ui {
class MainWindow;
}

class QGraphicsScene;
class QGraphicsItem;
class HistoryModel;
class Network;

class MainWindow : public QMainWindow, public Chess::IObserver
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

	int GetCellSize() const;
	QPoint PosToPixPos(const Chess::Pos& pos) const;
	Chess::Pos PixPosToPos(const QPoint& pos) const;
	Chess::Game*  GetGame() const { return game_.get(); }
	Chess::History*  GetHistory() const { return history_.get(); }
	QGraphicsScene* GetScene() {return boardScene_;}
    Chess::Color GetPlayer() const {return player_;}

public slots:
	void OnPieceMousePress(PieceItem&);
	void OnPieceMouseRelease(PieceItem&);
	void OnPieceAdded(Chess::Piece& piece) override;
	void OnPieceMoved(Chess::Piece& piece) override;
	void OnGameOver(Chess::Color color) override;
	void OnRollbackClick();
	void OnNewgameClick();
	void OnSavegameClick();
    void OnLoadgameClick();
	void OnHostGame();
	void OnConnectToGame();
    void OnAutomatch();
	void OnNetworkError(QAbstractSocket::SocketError error);

private:

	void PaintBoard();
	void CreateMovementBeacons();
	void ShowMovementBeacons(const Chess::Positions&);
	void HideMovementBeacons();

	Ui::MainWindow* ui_ = nullptr;
	QGraphicsScene* boardScene_ = nullptr; // Сцена для доски
	int cellSize_; // Размер клетки
	std::unique_ptr<Chess::Game> game_;
	std::unique_ptr<Chess::History> history_;
	std::vector<QGraphicsItem*> movementBeacons_;
	HistoryModel* historyModel_ = nullptr;
	Network* network_ = nullptr;
    Chess::Color player_ = (Chess::Color)((char)Chess::Color::Black | (char)Chess::Color::White);
};


