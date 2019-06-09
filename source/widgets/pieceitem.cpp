#include "pieceitem.h"
#include "game/game.h"
#include "mainwindow.h"

PieceItem::PieceItem( QObject *parent, Chess::Piece& piece, MainWindow& window)
	:QObject(parent)
	, piece_(&piece)
	, window_(&window)
{
	piece_->GetGame()-> RegisterObserver(*this); // 'this' is a pointer to object of PieceItem class inherited from IObserver

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

	UpdatePosition();
	setAcceptHoverEvents(true);
}

PieceItem::~ PieceItem()
{
	piece_->GetGame()-> UnregisterObserver(*this);
}

void PieceItem::OnPieceMoved(Chess::Piece& piece)
{
	if (&piece == piece_)
		UpdatePosition();
}

void PieceItem::mousePressEvent(QGraphicsSceneMouseEvent*)
{
	emit PieceMousePress(*this);
}

void PieceItem::UpdatePosition()
{
	setPos(window_->PosToPixPos(piece_->GetPos()));
}













