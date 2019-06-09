#include "graphicsview.h"
#include "widgets/pieceitem.h"
#include "widgets/mainwindow.h"
#include <QMouseEvent>
#include <QDebug>

GraphicsView::GraphicsView(QWidget *parent)
	: QGraphicsView(parent)
{

}

void GraphicsView::mouseMoveEvent(QMouseEvent* event)
{
	QGraphicsView::mouseMoveEvent(event);

	if (currentlyDragging_)
	{
		auto midsize = (currentlyDragging_->boundingRect().size() * 0.5).toSize();
		currentlyDragging_->setPos(event->pos() - QPoint(midsize.width(), midsize.height())); // новое положение во время движения мыши
	}
}


void GraphicsView:: mouseReleaseEvent(QMouseEvent* event)
{
	this->setCursor(QCursor(Qt::ArrowCursor));
	if (!currentlyDragging_)
		return;

	assert(mainWindow_);
	auto chess_pos = mainWindow_->PixPosToPos(event->pos());
	currentlyDragging_->GetPiece().SetPos(chess_pos);
	emit OnPieceMouseRelease(*currentlyDragging_);
	currentlyDragging_ = nullptr;

}

void GraphicsView::OnPieceMousePress(PieceItem& item)
{
	this->setCursor(QCursor(Qt::ClosedHandCursor));
	currentlyDragging_ = &item;
}
