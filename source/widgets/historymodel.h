#pragma once

#include <QAbstractItemModel>
#include "game/gamecommon.h"

class HistoryModel : public QAbstractItemModel
{
Q_OBJECT

public:

	HistoryModel(QObject* parent);

	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &index) const override;

	void SetHistory(Chess::History* history);

protected:

	Chess::History* history_ = nullptr;
};
