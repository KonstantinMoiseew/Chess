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

    painter->drawText(option.rect.topLeft() + QPoint(35,15), command->ToString().c_str());
	painter->restore();
}
