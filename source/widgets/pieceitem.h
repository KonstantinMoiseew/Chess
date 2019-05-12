#pragma once
#include <QGraphicsPixmapItem>
#include "game/iobserver.h"

class MainWindow;

class PieceItem:   public Chess::IObserver, public QGraphicsPixmapItem
{
public:

	PieceItem(Chess::Piece& piece, MainWindow& window);
	~PieceItem();

protected:

	Chess::Piece* piece_ = nullptr;
	MainWindow* window_ = nullptr;
};
