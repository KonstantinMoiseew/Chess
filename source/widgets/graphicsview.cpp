#include "graphicsview.h"
#include "game/commands.h"
#include "widgets/pieceitem.h"
#include "widgets/mainwindow.h"
#include <QMouseEvent>
#include <QDebug>
#include"labelbotton.h"

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
	}
}


void GraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
	setCursor(QCursor(Qt::ArrowCursor));
	if (!currentlyDragging_)
		return;

	assert(mainWindow_);

    if ((char)mainWindow_->GetPlayer() & (char)mainWindow_->GetGame()->GetPlayerTurn())
    {
        auto chess_pos = mainWindow_->PixPosToPos(event->pos());
        auto& piece =  currentlyDragging_->GetPiece();
        auto game = mainWindow_->GetGame();

        Chess::PieceType promotion_type = Chess::PieceType::Queen;
        if (game->IsPromotionMove(piece, chess_pos))
        {
            // TODO: Dialog window
            // promotion_type = ...
        }

        auto command = game->CreateCommand(piece, chess_pos, promotion_type);

        if (command)
        {
            if (!mainWindow_->GetHistory()->Execute(*game, command))
                currentlyDragging_->UpdatePosition();
        }
    }
    else
        currentlyDragging_->UpdatePosition();

	emit OnPieceMouseRelease(*currentlyDragging_);

	currentlyDragging_ = nullptr;
}

void GraphicsView::OnPieceMousePress(PieceItem& item)
{
	setCursor(QCursor(Qt::ClosedHandCursor));
	currentlyDragging_ = &item;
}



