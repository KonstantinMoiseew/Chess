#include <QGraphicsView>
#include "widgets/pieceitem.h"
#include "widgets/mainwindow.h"
#include <QPushButton>
class PieceItem;
class MainWindow;

class GraphicsView : public QGraphicsView // It is added to ui_Main_Window. All PieceItemes will be added here
{
	Q_OBJECT
public:

	GraphicsView(QWidget *parent = nullptr);

	void SetWindow(MainWindow& window) {mainWindow_ = &window;}

signals:
	void OnPieceMouseRelease(PieceItem&);

public slots:
	void OnPieceMousePress(PieceItem&);

protected:

	void mouseMoveEvent(QMouseEvent * event) override;
	void mouseReleaseEvent(QMouseEvent * event) override;

	PieceItem* currentlyDragging_ = nullptr;
	MainWindow* mainWindow_ = nullptr;

};
