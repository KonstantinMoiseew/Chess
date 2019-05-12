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

	// Выключаем прокрутку чтобы не мешала
	ui_->boardView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui_->boardView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	ui_->boardView->setFrameStyle(QFrame::NoFrame);

	// game_.reset(new Chess::Game);
	game_ = new Chess::Game();
	game_->RegisterObserver(*this);

	// Раскрашиваем доску
	PaintBoard();

	//SetupFiguresWhite();
	game_->ArrangeFigures();
}

MainWindow::~MainWindow()
{
	game_->UnregisterObserver(*this);
	// Внимание, item-ы не работают с принятой в Qt древовидной системой управления памятью!
	// Нужно удалить вручную!
	for (auto& item : boardScene_->items())
		delete item;

	delete ui_;
}

void MainWindow::OnPieceAdded(Chess::Piece& piece)
{
	auto item = new PieceItem(piece, *this);
	boardScene_->addItem(item);
}

int MainWindow::GetCellSize() const
{
	return cellSize_;
}

QPoint MainWindow::PosToPixPos(const Chess::Pos& pos) const
{
	return QPoint (pos.x_ * cellSize_, (Chess::BoardSize - pos.y_ - 1) * cellSize_);
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
