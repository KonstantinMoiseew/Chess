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

	UpdatePosition();
	//setAcceptHoverEvents(true);
}

void PieceItem::OnPieceMoved(Chess::Piece& piece) //виртуальный метод, который переопределен в классе pieceItem. Объект этого класса храниться в game (в векторе с наблюдателями).
{
	if (&piece == piece_)
		UpdatePosition();  // на доске (на view) устанавливается фигура
}

void PieceItem::OnPieceRemoved(Chess::Piece& piece)
{
	if (&piece == piece_)
	{
		//OnUnregistered();
		 delete this;
	}
}



void PieceItem::mousePressEvent(QGraphicsSceneMouseEvent*  ) // при нажатии на item он будут захвачен курсором, перегружаем библиотечную функцию
{
	emit PieceMousePress(*this);
}

void PieceItem::UpdatePosition()
{
	setPos(window_->PosToPixPos(piece_->GetPos())); //библиотечная функция QGraphicsPixmapItem - меняет визуальное положение item на доске
}













