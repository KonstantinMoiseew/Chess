#ifndef BUTTONCHANGEPAWN_H
#define BUTTONCHANGEPAWN_H
#include<QLabel>
class LabelBotton: public QLabel
{
	Q_OBJECT
public:
	explicit LabelBotton(QWidget *parent,QPixmap px);
	explicit LabelBotton(QWidget *parent);
signals:
	void clicked();

public slots:

protected:
	void mousePressEvent(QMouseEvent *ev) override;
	void mouseReleaseEvent(QMouseEvent *ev) override;
	void enterEvent(QEvent *) override;
	void leaveEvent(QEvent *) override;

private:
	QPixmap pixmap;
};

#endif // BUTTONCHANGEPAWN_H
