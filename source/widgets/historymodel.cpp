#include "historymodel.h"
#include "game/history.h"
#include <QPixmap>

HistoryModel::HistoryModel(QObject* parent)
	: QAbstractItemModel (parent)
{
}

int HistoryModel::columnCount(const QModelIndex&) const
{
	return 2;
}

int HistoryModel::rowCount(const QModelIndex&) const
{
	if (!history_)
		return 1;

	return (history_->GetSize() + 1) / 2;
}

QVariant HistoryModel::data(const QModelIndex&, int) const
{
    return QVariant();
}

QModelIndex HistoryModel::index(int row, int column, const QModelIndex&) const
{
	return createIndex(row, column, history_ ? history_->GetCommand(row * 2 + column) : nullptr);
}

QModelIndex HistoryModel::parent(const QModelIndex&) const
{
	return QModelIndex();
}

void HistoryModel::SetHistory(Chess::History* history)
{
	history_ = history;
	Refresh();
}

void HistoryModel::Refresh()
{
	beginResetModel(); //When a model is reset it means that any previous data reported from the model is now invalid and has to be queried for again.
										 // This also means that the current item and any selected items will become invalid.
	endResetModel();
}
