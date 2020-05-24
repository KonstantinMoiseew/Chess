#pragma once
#include <QMainWindow>
#include "game/iobserver.h"
#include "game/gamecommon.h"
#include "game/game.h"
#include "game/history.h"
#include "pieceitem.h"



namespace Ui {
class MainWindow;
}

class QGraphicsScene;
class QGraphicsItem;
class HistoryModel;

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

public slots:
	void OnPieceMousePress(PieceItem&);
	void OnPieceMouseRelease(PieceItem&);
	void OnPieceAdded(Chess::Piece& piece) override;
	void OnPieceMoved(Chess::Piece& piece) override;
	void OnGameOver(Chess::Color color) override;
	void OnShowChose(Chess::Piece& piece) override;
	void OnRollbackClick();
	void OnNewgameClick();
	void OnSavegameClick();
	void OnCangeFigure();

private:

	void PaintBoard();
	void CreateMovementBeacons();
	void ShowMovementBeacons(const Chess::Positions&);
	void HideMovementBeacons();
	void CreateChoseWindow();
	void HideChoseWindow();
	void ShowChoseWindow();


	Ui::MainWindow* ui_ = nullptr;
	QGraphicsScene* boardScene_ = nullptr; // Сцена для доски
	int cellSize_; // Размер клетки
	std::unique_ptr<Chess::Game> game_;
	std::unique_ptr<Chess::History> history_;  //
	std::vector<QGraphicsItem*> movementBeacons_;
	HistoryModel* historyModel_ = nullptr;  //
};


