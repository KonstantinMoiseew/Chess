#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsItem>
#include "source/game/piece.h"
#include "source/widgets/pieceitem.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>


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


	game_.reset(new Chess::Game());
	game_->RegisterObserver(*this);

	// Раскрашиваем доску
	PaintBoard();

	CreateMovementBeacons();
	HideMovementBeacons();

	//SetupFiguresWhite();
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

void MainWindow::OnPieceMousePress(PieceItem& pieceItem)
{
	ShowMovementBeacons(pieceItem.GetPiece().GetMovement().GetAvailableMovement()); // Где устанавливается movement_ в каждом piece?
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

			const QColor color(128, 128, 128);

			QRectF rect(top_left, bottom_right);
			movementBeacons_.push_back(boardScene_->addEllipse(rect, Qt::NoPen, QBrush(color)));
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
