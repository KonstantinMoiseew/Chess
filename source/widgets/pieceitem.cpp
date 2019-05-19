#include "pieceitem.h"
#include "game/game.h"
#include "mainwindow.h"

PieceItem::PieceItem( QObject *parent, Chess::Piece& piece, MainWindow& window)
	  :QObject(parent)
	, piece_(&piece)
	, window_(&window)
{
	piece_->GetGame()-> RegisterObserver(*this);

	QString image_name = ":/graphics/pieces/";
	switch (piece.GetType())
	{
	case Chess::Type::Pawn: image_name += "pawn"; break;
	case Chess::Type::Knight: image_name += "knight"; break;
	case Chess::Type::Bishop: image_name += "bishop"; break;
	case Chess::Type::Rook : image_name += "rook"; break;
	case Chess::Type::Queen: image_name += "queen"; break;
	case Chess::Type::King: image_name += "king"; break;
	}

	if (piece.GetColor() == Chess::Color::White)
		image_name += "_white";
	else
		image_name += "_black";

	image_name += ".png";

	QPixmap pixmap(image_name);
	pixmap = pixmap.scaled(window_->GetCellSize(), window_->GetCellSize());
	setPixmap(pixmap);

	setPos(window_->PosToPixPos(piece.GetPos()));
}

PieceItem::~ PieceItem()
{
	piece_->GetGame()-> UnregisterObserver(*this);
}




	QRectF PieceItem:: boundingRect() const
	{
		int cell_size=window_->GetCellSize();
		return QRectF(0,0,cell_size,cell_size);
	}

	/*void PieceItem:: paint(QPainter *painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
	{
		painter->setPen(Qt::black);
		painter->setBrush(Qt::green);
		painter->drawRect(-30,-30,60,60);
		Q_UNUSED(option);
		Q_UNUSED(widget);

	}*/

	//void PieceItem:: mouseMoveEvent(QGraphicsSceneMouseEvent * event)
	//{
	//	this->setPos(mapToScene(event->pos()));
	//}

	/*void PieceItem:: mousePressEvent(QGraphicsSceneMouseEvent * event)
	{
		this->setCursor(QCursor(Qt::ClosedHandCursor));
		Q_UNUSED(event);
	}*/

	/*void PieceItem:: mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
	{
		this->setCursor(QCursor(Qt::ArrowCursor));
		Q_UNUSED(event);
	}*/

















