#include "pieceitem.h"
#include "game/game.h"
#include "mainwindow.h"
#include "widgettools.h"

PieceItem::PieceItem( QObject *parent, Chess::Piece& piece, MainWindow& window)
	: QObject(parent)
	, piece_(&piece)
	, window_(&window)
{
	piece_->GetGame()-> RegisterObserver(*this);

	QString image_name = GetPiecePixmapPath(piece_->GetType(), piece_->GetColor());

	QPixmap pixmap(image_name);
	pixmap = pixmap.scaled(window_->GetCellSize(), window_->GetCellSize());
	setPixmap(pixmap); //

	UpdatePosition();
}

void PieceItem::OnPieceMoved(Chess::Piece& piece)
{
	if (&piece == piece_)
		UpdatePosition();
}

void PieceItem::OnPieceAboutToBeRemoved(Chess::Piece& piece)
{
	if (&piece == piece_)
	{
		 delete this;
	}
}

void PieceItem::mousePressEvent(QGraphicsSceneMouseEvent*  ) // при нажатии на item он будут захвачен курсором, перегружаем библиотечную функцию
{
	emit PieceMousePress(*this);
}

void PieceItem::UpdatePosition()
{
	setPos(window_->PosToPixPos(piece_->GetPos())); //библиотечная функция QGraphicsPixmapItem - меняет визуальное положение item на доске и фигура встает именно на клетку (из-за пересчета координат в intы)
}













