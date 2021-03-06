#pragma once
#include <QObject>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include<QDebug>
#include<QCursor>
#include <QGraphicsPixmapItem>
#include<QGraphicsItem>
#include "game/iobserver.h"

class MainWindow;

class PieceItem:   public QObject, public QGraphicsPixmapItem, public Chess::IObserver
{
	Q_OBJECT
public:

	explicit PieceItem(QObject *parent, Chess::Piece& piece, MainWindow& window);

	Chess::Piece& GetPiece() const { return *piece_; }

	void OnPieceMoved(Chess::Piece& piece) override;
	void OnPieceAboutToBeRemoved(Chess::Piece& piece) override;
	void UpdatePosition();

signals:
	void PieceMousePress(PieceItem&);

protected:

	void mousePressEvent(QGraphicsSceneMouseEvent * event) override;

	Chess::Piece* piece_ = nullptr;
	MainWindow* window_ = nullptr;
};




















