#include "historymodel.h"
#include "game/history.h"

HistoryModel::HistoryModel(QObject* parent)
	: QAbstractItemModel (parent)
{
}

int HistoryModel::columnCount(const QModelIndex&) const
{
	return 1;
}

int HistoryModel::rowCount(const QModelIndex&) const
{
	if (!history_)
		return 0;

	return history_->GetSize();
}

QVariant HistoryModel::data(const QModelIndex& index, int role) const
{
	switch(role)
	{
	case Qt::DisplayRole: return reinterpret_cast<Chess::ICommand*>(index.internalPointer())->ToString().c_str();
	default: return QVariant();
	}
}

QModelIndex HistoryModel::index(int row, int column, const QModelIndex&) const
{
	return createIndex(row, column, history_ ? history_->GetCommand(row) : nullptr);
}

QModelIndex HistoryModel::parent(const QModelIndex&) const
{
	return QModelIndex();
}

void HistoryModel::SetHistory(Chess::History* history)
{
	beginResetModel();
	history_ = history;
	endResetModel();
}
