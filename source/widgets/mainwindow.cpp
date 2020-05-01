#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "game/piece.h"
#include "widgets/pieceitem.h"
#include "widgets/historymodel.h"
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsView>
#include"historydelegate.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui_(new Ui::MainWindow)
{
	ui_->setupUi(this);

	cellSize_ = ui_->boardView->width() / 8; // Размер клетки в 8 раз меньше размера доски

	boardScene_ = new QGraphicsScene(this); // Создаём сцену для доски. Привязываем её время жизни к окну (this)
	ui_->boardView->setScene(boardScene_); // boardView (QGraphicsView) будет визуализировать эту сцену
	ui_->boardView->SetWindow(*this);


	// Выключаем прокрутку чтобы не мешала
	ui_->boardView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui_->boardView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	ui_->boardView->setFrameStyle(QFrame::NoFrame);

	boardScene_->setItemIndexMethod(QGraphicsScene::NoIndex); // настраиваем индексацию элементов
	ui_->boardView->setRenderHint(QPainter::Antialiasing);
	ui_->boardView->setCacheMode(QGraphicsView::CacheBackground);
	ui_->boardView->setViewportUpdateMode((QGraphicsView::BoundingRectViewportUpdate));
	ui_->boardView->setMouseTracking(true); //switch on tracking of the mouse without pressing
	connect(ui_->boardView, &GraphicsView::OnPieceMouseRelease, this, &MainWindow::OnPieceMouseRelease);
	connect(ui_->rollbackButton, &QPushButton::clicked, this, &MainWindow::OnRollbackClick);
	connect(ui_->newgameButton, &QPushButton::clicked, this, &MainWindow::OnNewgameClick);
	connect(ui_->savegameButton, &QPushButton::clicked, this, &MainWindow::OnSavegameClick);

	historyModel_ = new HistoryModel(this);
	HistoryDelegate * historedelegate_ = new HistoryDelegate;
	ui_->historyView->setModel(historyModel_);// во view устанавливаем указатель на модель
	ui_->historyView->setItemDelegate(historedelegate_);
	ui_->historyView->horizontalHeader()->setStretchLastSection(true);
	ui_->historyView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	ui_->historyView->setColumnWidth(0, ui_->historyView->width() / 2);

	game_.reset(new Chess::Game);
	game_->RegisterObserver(*this);
	history_.reset(new Chess::History);

	historyModel_->SetHistory(history_.get());

	// Раскрашиваем доску
	PaintBoard();

	CreateMovementBeacons();
	HideMovementBeacons();
	ui_->checkmateLabel->hide();

	game_->ArrangeFigures();
}

MainWindow::~MainWindow()
{
	delete ui_;
}

void MainWindow::OnPieceAdded(Chess::Piece& piece)
{
	auto item = new PieceItem(this, piece, *this);
	boardScene_->addItem(item);
	connect(item, &PieceItem::PieceMousePress, ui_->boardView, &GraphicsView::OnPieceMousePress);
	connect(item, &PieceItem::PieceMousePress, this, &MainWindow::OnPieceMousePress);
}

void MainWindow::OnRollbackClick()
{
	history_->RollbackLast(*GetGame());
}

void  MainWindow::OnNewgameClick()
{

	game_->RemoveAllPieces();
	history_->ClearHistory();
	historyModel_->Refresh();
	game_->ArrangeFigures();

}


void MainWindow::OnSavegameClick()
{

}

void MainWindow::OnPieceMousePress(PieceItem& pieceItem)
{
	ShowMovementBeacons(pieceItem.GetPiece().GetMovement().GetAvailableMovement());
}

void MainWindow::OnPieceMouseRelease(PieceItem&)
{
	HideMovementBeacons();
}

int MainWindow::GetCellSize() const
{
	return cellSize_;
}

QPoint MainWindow::PosToPixPos(const Chess::Pos& pos) const
{
	return QPoint (pos.x_ * cellSize_, (Chess::BoardSize - pos.y_ - 1) * cellSize_);
}

Chess::Pos MainWindow::PixPosToPos(const QPoint& pos) const
{
	return Chess::Pos(pos.x() / cellSize_, Chess::BoardSize - 1 - pos.y() / cellSize_);
}

void MainWindow::PaintBoard()
{
	for (int i = 0; i < Chess::BoardSize; i++)
	{
		for (int j = 0; j < Chess::BoardSize; j++)
		{
			int x = i * cellSize_;
			int y = j * cellSize_;

			QPointF top_left(x, y);
			QPointF bottom_right(x + cellSize_, y + cellSize_);

			const QColor dark(68, 37, 33);
			const QColor light(231, 198, 114);

			QColor color = (i + j) % 2 ? dark : light; // Чередуем цвета квадратов

			QRectF rect(top_left, bottom_right);
			boardScene_->addRect(rect, Qt::NoPen, QBrush(color)); // Добавляем квадраты на доску
		}
	}


}

void MainWindow::CreateMovementBeacons()
{
	for (int i = 0; i < Chess::BoardSize; i++)
	{
		for (int j = Chess::BoardSize - 1; j >= 0; j--)
		{
			int x = i * cellSize_;
			int y = j * cellSize_;

			QPointF top_left(x, y);
			QPointF bottom_right(x + cellSize_, y + cellSize_);

			const QColor color_border(0, 255, 0);
			const QColor color_inner(0, 255, 0, 64);

			QRectF rect(top_left, bottom_right);
			movementBeacons_.push_back(boardScene_->addRect(rect, QPen(QBrush(color_border), 3, Qt::PenStyle::DashLine), QBrush(color_inner)));
		}
	}
}

void MainWindow::ShowMovementBeacons(const Chess::Positions& positions)
{
	for (auto& pos : positions)
	{
		unsigned int index = static_cast<unsigned int>(pos.x_ * Chess::BoardSize + pos.y_);
		movementBeacons_[index]->show();
	}
}

void MainWindow::HideMovementBeacons()
{
	for (auto& beacon : movementBeacons_)
	{
		beacon->hide();
	}
}

void MainWindow::OnPieceMoved(Chess::Piece&)
{
	historyModel_->Refresh();
}

void MainWindow::OnGameOver(Chess::Color)
{
	ui_->checkmateLabel->show();
}
