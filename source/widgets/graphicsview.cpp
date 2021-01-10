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
        Chess::ICommand* command = nullptr;
        auto& piece =  currentlyDragging_->GetPiece();
        if (piece.GetType() == Chess::PieceType::King &&
            (chess_pos - piece.GetPos()).x_ == 2 &&
            (piece.GetPos().y_ == 0 || piece.GetPos().y_ == 7))
        {
            std::vector<Chess::ICommandUnPtr> commands;
            commands.push_back(Chess::ICommandUnPtr(new Chess::MoveCommand(piece, chess_pos)));
            auto rook = mainWindow_->GetGame()->FindPieceAt({7, 0});
            assert(rook && rook->GetType() == Chess::PieceType::Rook);
            commands.push_back(Chess::ICommandUnPtr(new Chess::MoveCommand(*rook, chess_pos - Chess::Pos(1, 0))));
            command = new Chess::CompoundCommand(std::move(commands));
        }
        else
            command = new Chess::MoveCommand(piece, chess_pos);

        if (!mainWindow_->GetHistory()->Execute(*mainWindow_->GetGame(), command))
            currentlyDragging_->UpdatePosition();
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



