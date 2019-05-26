#include <QGraphicsView>

class PieceItem;
class MainWindow;

class GraphicsView : public QGraphicsView
{
public:

	GraphicsView(QWidget *parent = nullptr);

	void SetWindow(MainWindow& window) {mainWindow_ = &window;}

public slots:
	void OnPieceMousePress(PieceItem&);

protected:

	void mouseMoveEvent(QMouseEvent * event) override;
	void mouseReleaseEvent(QMouseEvent * event) override;

	PieceItem* currentlyDragging_ = nullptr;
	MainWindow* mainWindow_ = nullptr;
};
