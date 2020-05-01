#pragma once
#include<QStyledItemDelegate>
#include<QPixmap>
#include<QPainter>
#include "game/gamecommon.h"
#include "historymodel.h"
#include "game/history.h"
class  HistoryDelegate: public QStyledItemDelegate
{
public:
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};
