#include "graphicsview.h"
#include "widgets/pieceitem.h"
#include "widgets/mainwindow.h"
#include <QMouseEvent>
#include <QDebug>

GraphicsView::GraphicsView(QWidget *parent)
	: QGraphicsView(parent)
{

}

void GraphicsView::mouseMoveEvent(QMouseEvent* event)//  event - содержит в себе QPoint. Функция вызывается всякий раз при шевелении мыши.
{
	QGraphicsView::mouseMoveEvent(event);

	if (currentlyDragging_)
	{
		auto midsize = (currentlyDragging_->boundingRect().size() * 0.5).toSize();
		currentlyDragging_->setPos(event->pos() - QPoint(midsize.width(), midsize.height()));  // новое положение во время движения мыши
	}// setPos - библиотечная функция, устанавливает фигуру на доске, т.е на сцене.
}


void GraphicsView:: mouseReleaseEvent(QMouseEvent* event)
{
	setCursor(QCursor(Qt::ArrowCursor));
	if (!currentlyDragging_)
		return;

	assert(mainWindow_);
	auto chess_pos = mainWindow_->PixPosToPos(event->pos());
	currentlyDragging_->GetPiece().SetPos(chess_pos); //устанавливается новые координаты в piece, и из этого же piece вызывается функция OnPieceMoved, которая уже устанавливает item на view
	emit OnPieceMouseRelease(*currentlyDragging_);
	currentlyDragging_ = nullptr;

}

void GraphicsView::OnPieceMousePress(PieceItem& item)
{
	setCursor(QCursor(Qt::ClosedHandCursor));
	currentlyDragging_ = &item;
}



