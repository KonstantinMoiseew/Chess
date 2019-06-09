#pragma once
#include <QMainWindow>
#include "source/game/iobserver.h"
#include "source/game/gamecommon.h"
#include "source/game/game.h"

namespace Ui {
class MainWindow;
}

class QGraphicsScene;

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
	Chess::Game *  GetGame() const { return game_; }

	public slots:
	void on_pushButton_clicked()
	{
		game_->MovingPiece();
	}

private:

    void PaintBoard();


	Ui::MainWindow* ui_;
	QGraphicsScene* boardScene_; // Сцена для доски
	int cellSize_; // Размер клетки
	Chess::Game * game_;
private slots:
};


