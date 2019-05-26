#include "graphicsview.h"
#include "widgets/pieceitem.h"
#include "widgets/mainwindow.h"
#include <QMouseEvent>
#include <QDebug.h>

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
		currentlyDragging_->setPos(event->pos() - QPoint(midsize.width(), midsize.height()));
	}
}


void GraphicsView:: mouseReleaseEvent(QMouseEvent* event)
{
	if (!currentlyDragging_)
		return;

	assert(mainWindow_);
	auto chess_pos = mainWindow_->PixPosToPos(event->pos());
	currentlyDragging_->GetPiece().SetPos(chess_pos);
	currentlyDragging_ = nullptr;
}

void GraphicsView::OnPieceMousePress(PieceItem& item)
{
	currentlyDragging_ = &item;
}
