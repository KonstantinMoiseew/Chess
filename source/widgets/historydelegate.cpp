#include"historydelegate.h"
#include "widgettools.h"

void HistoryDelegate:: paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
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
	painter->translate(option.rect.topLeft());
	painter->drawPixmap(QPoint(),pixmap.scaledToWidth(30,  Qt::FastTransformation));
	painter->translate(QPoint(20.0, 0.0));
	painter->drawText(QPoint(0, option.rect.bottom()), command->ToString().c_str());
	painter->translate(QPoint(40.0, 0.0));
	painter->drawPixmap(QPoint(),pixmap.scaledToWidth(30,  Qt::FastTransformation));
	painter->restore();
}
