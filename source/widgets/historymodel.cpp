#include "historymodel.h"
#include "game/history.h"

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

QVariant HistoryModel::data(const QModelIndex& index, int role) const
{
	if (!index.internalPointer())
		return QVariant();

	switch(role)
	{
	case Qt::DisplayRole: return reinterpret_cast<Chess::ICommand*>(index.internalPointer())->ToString().c_str();
	default: return QVariant();
	}
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
	beginResetModel();
	endResetModel();
}
