#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cellSize = ui->boardView->width() / 8; // Размер клетки в 8 раз меньше размера доски

    boardScene = new QGraphicsScene(this); // Создаём сцену для доски. Привязываем её время жизни к окну (this)
    ui->boardView->setScene(boardScene); // boardView (QGraphicsView) будет визуализировать эту сцену

    // Выключаем прокрутку чтобы не мешала
    ui->boardView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->boardView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->boardView->setFrameStyle(QFrame::NoFrame);

    // Раскрашиваем доску
    PaintBoard();

    SetupFigures();

    ui->boardView->setSceneRect(QRectF(0, 0, ui->boardView->width(), ui->boardView->height()));
}

MainWindow::~MainWindow()
{
    // Внимание, item-ы не работают с принятой в Qt древовидной системой управления памятью!
    // Нужно удалить вручную!
    for (auto& item : boardScene->items())
        delete item;

    delete ui;
}

void MainWindow::PaintBoard()
{
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            int x = i * cellSize;
            int y = j * cellSize;

            QPointF top_left(x, y);
            QPointF bottom_right(x + cellSize, y + cellSize);

            const QColor dark(68, 37, 33);
            const QColor light(231, 198, 114);

            QColor color = (i + j) % 2 ? dark : light; // Чередуем цвета квадратов

            QRectF rect(top_left, bottom_right);
            boardScene->addRect(rect, Qt::NoPen, QBrush(color)); // Добавляем квадраты на доску
        }
}

void MainWindow::SetupFigures()
{
    // Создаем два ряда фигур
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 8; j++)
        {
            int x = j * cellSize;
            int y = (i + 6) * cellSize;

            QPointF top_left(x, y);
            QPointF bottom_right(x + cellSize, y + cellSize);

            QPixmap pixmap("D:/GitHub/Chess/graphics/pieces/pawn_white.png");
            pixmap = pixmap.scaled(cellSize, cellSize);
            auto item = boardScene->addPixmap(pixmap);
            item->setPos(top_left);
        }
}
