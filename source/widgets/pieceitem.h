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

class PieceItem:   public QObject, public Chess::IObserver, public QGraphicsPixmapItem
{
	Q_OBJECT
public:

	explicit PieceItem(QObject *parent, Chess::Piece& piece, MainWindow& window);
	~PieceItem() override;


protected:

	Chess::Piece* piece_ = nullptr;
	MainWindow* window_ = nullptr;

signals:

private:


	QRectF boundingRect() const override;

	/*void paint(QPainter *painter, const QStyleOptionGraphicsItem * option, QWidget * widget) override
	{
		painter->setPen(Qt::black);
		painter->setBrush(Qt::green);
		painter->drawRect(-30,-30,60,60);
		Q_UNUSED(option);
		Q_UNUSED(widget);

	}*/

	void mouseMoveEvent(QGraphicsSceneMouseEvent * event) override
	{
		this->setPos(mapToScene(event->pos()));
	}

	void mousePressEvent(QGraphicsSceneMouseEvent * event) override
	{
		this->setCursor(QCursor(Qt::ClosedHandCursor));
		Q_UNUSED(event);
	}
	void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override
	{
		this->setCursor(QCursor(Qt::ArrowCursor));
		Q_UNUSED(event);
	}
public slots:
};
