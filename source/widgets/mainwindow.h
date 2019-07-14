#pragma once
#include <QMainWindow>
#include "game/iobserver.h"
#include "game/gamecommon.h"
#include "game/game.h"
#include "pieceitem.h"

namespace Ui {
class MainWindow;
}

class QGraphicsScene;
class QGraphicsItem;

class MainWindow : public QMainWindow, public Chess::IObserver
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void OnPieceAdded(Chess::Piece&) override;

	int GetCellSize() const;

	QPoint PosToPixPos(const Chess::Pos& pos) const;
	Chess::Pos PixPosToPos(const QPoint& pos) const;
	Chess::Game *  GetGame() const { return game_.get(); }

public slots:
	void OnPieceMousePress(PieceItem&);
	void OnPieceMouseRelease(PieceItem&);

private:

	void PaintBoard();
	void CreateMovementBeacons();
	void ShowMovementBeacons(const Chess::Positions&);
	void HideMovementBeacons();

	Ui::MainWindow* ui_;
	QGraphicsScene* boardScene_; // Сцена для доски
	int cellSize_; // Размер клетки
	std::unique_ptr<Chess::Game> game_;
	std::vector<QGraphicsItem*> movementBeacons_;
};


