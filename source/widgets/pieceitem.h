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
	~PieceItem() override;

	Chess::Piece& GetPiece() const { return *piece_; }

	void OnPieceMoved(Chess::Piece& piece) override;

signals:
	void PieceMousePress(PieceItem&);

protected:

	void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
	void UpdatePosition();


	Chess::Piece* piece_ = nullptr;
	MainWindow* window_ = nullptr;
};




















