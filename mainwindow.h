#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QGraphicsScene;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

    void PaintBoard();
    void SetupFigures();

    Ui::MainWindow* ui;
    QGraphicsScene* boardScene; // Сцена для доски
    int cellSize; // Размер клетки
};

#endif // MAINWINDOW_H
