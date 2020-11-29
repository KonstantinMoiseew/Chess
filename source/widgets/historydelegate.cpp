#include"historydelegate.h"
#include "widgettools.h"
#include<QBrush>

void HistoryDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (!index.internalPointer())
	{
		QStyledItemDelegate::paint(painter, option, index);
		return;
	}

	auto command = reinterpret_cast<Chess::ICommand*>(index.internalPointer());

	QString image_name = GetPiecePixmapPath(command->GetPieceType(), command->GetPieceColor());
	QPixmap pixmap(image_name);

	painter->save();
	painter->translate(option.rect.topLeft()+QPoint(0,0));
	painter->drawPixmap(QPoint(0,-5),pixmap.scaledToWidth(30,  Qt::FastTransformation));

    painter->drawText(35,15, command->ToString().c_str());

	if(command->CheckEmptyEnemy())
	{

	QString image_name = GetPiecePixmapPath(command->GetCapturedPieceType(), command->GetCapturedPieceColor());
	QPixmap pixmap(image_name);
	painter->drawPixmap(QPoint(75,-5),pixmap.scaledToWidth(31,  Qt::FastTransformation));
	}

	if(command->KingUnderAttak())
	{
		//QString image_name = GetPiecePixmapPath(command->GetPieceType(), command->GetPieceColor());
		QString image_name = ":/graphics/pieces/king";
		if(command->GetPieceColor()==Chess::Color::Black)
		{
			image_name += "_white.png";
		}
		else image_name += "_black.png";
		QPixmap pixmap(image_name);

		painter->drawPixmap(QPoint(92,0),pixmap.scaledToWidth(22,  Qt::FastTransformation));
		QColor color(231, 198, 114);
		QBrush brush(color,  Qt::Dense5Pattern );
		painter->fillRect(0, 0, 30, 30, brush);
	}

	painter->restore();
}
