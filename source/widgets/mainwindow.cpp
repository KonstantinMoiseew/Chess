#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "game/piece.h"
#include "widgets/pieceitem.h"
#include "widgets/historymodel.h"
#include "widgets/historydelegate.h"
#include "widgets/labelbotton.h"
#include "widgets/network.h"
#include <fstream>
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsView>
#include <QMessageBox>
#include <QMetaEnum>


bool g_DBG_Play_As_White = true;
bool g_DBG_Automatch = false;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui_(new Ui::MainWindow)
{
	ui_->setupUi(this);

    cellSize_ = ui_->boardView->width() / Chess::BoardSize; // Размер клетки в 8 раз меньше размера доски

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
	//ShowChoseWindow();

    network_ = new Network(this, game_.get(), history_.get());
	connect(network_, &Network::NetworkError, this, &MainWindow::OnNetworkError);
    game_->RegisterObserver(*network_);

    connect(ui_->actionNew, &QAction::triggered, this, &MainWindow::OnNewgameClick);
    connect(ui_->actionSave, &QAction::triggered, this, &MainWindow::OnSavegameClick);
    connect(ui_->actionLoad, &QAction::triggered, this, &MainWindow::OnLoadgameClick);
	connect(ui_->actionHost, &QAction::triggered, this, &MainWindow::OnHostGame);
	connect(ui_->actionConnect, &QAction::triggered, this, &MainWindow::OnConnectToGame);
    connect(ui_->actionAutomatch, &QAction::triggered, this, &MainWindow::OnAutomatch);

    ui_->DBGplayAsWhite->setChecked(g_DBG_Play_As_White);

    if (g_DBG_Automatch)
        OnAutomatch();
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
	ui_->checkmateLabel->hide();
	game_->ArrangeFigures();
}

void MainWindow::OnSavegameClick()
{    
    obytestream obs;
    for (int i = 0; i < history_->GetSize(); i++)
    {
        auto command = history_->GetCommand(i);
        Chess::ICommand::Serialize(obs, command);
    }

    std::ofstream f("savegame.chess", std::ios::out | std::ios::binary);
    f.write(obs.GetBuffer().data(), obs.GetBuffer().size());
}

void MainWindow::OnLoadgameClick()
{
    std::vector<char> buffer;
    std::ifstream f("savegame.chess", std::ios::out | std::ios::binary);
    while(true)
    {
        int ch = f.get();
        if (ch==EOF)
            break;
        buffer.push_back((char)ch);
    }
    while(history_->GetSize())
    {
        history_->RollbackLast(*game_);
    }
    ibytestream stream(std::move(buffer));
    while (auto command = Chess::ICommand::Deserialize(stream))
    {
        history_->Execute(*game_, command);
    }
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

void MainWindow::OnHostGame()
{
	OnNewgameClick();
	network_->HostGame();
    player_ = Chess::Color::White;
}

void MainWindow::OnConnectToGame()
{
	OnNewgameClick();
	network_->ConnectToGame();
    player_ = Chess::Color::Black;
}

void MainWindow::OnAutomatch()
{
    OnNewgameClick();
    network_->Automatch();
    player_ = ui_->DBGplayAsWhite->isChecked() ? Chess::Color::White : Chess::Color::Black;
}

void MainWindow::OnNetworkError(QAbstractSocket::SocketError error)
{
	QMessageBox message_box;
	message_box.setText("Network error:" + QString(QMetaEnum::fromType<QAbstractSocket::SocketError>().valueToKey(error)));
	message_box.exec();
}
