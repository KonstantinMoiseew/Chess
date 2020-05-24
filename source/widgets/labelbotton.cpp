#include"labelbotton.h"
#include<QPainter>


LabelBotton:: LabelBotton(QWidget *parent, QPixmap px): QLabel(parent)
	{
	pixmap=px;
	this->setPixmap(pixmap);
	}

LabelBotton:: LabelBotton(QWidget *parent): QLabel(parent)
	{
	QString image_name = ":/graphics/pieces/knight_black.png";
	QPixmap pixmap(image_name);
	pixmap = pixmap.scaled(30, 30);
	//this->setPixmap(pixmap);
	}

void LabelBotton:: mousePressEvent(QMouseEvent *ev)
	{
	setCursor(QCursor(Qt::PointingHandCursor));
	this->move(x()+1,y()+1);
	emit clicked();
	}

void LabelBotton:: mouseReleaseEvent(QMouseEvent *ev)
	{
	setCursor(QCursor(Qt::ArrowCursor));
	this->move(x()-1,y()-1);

	}

void LabelBotton:: enterEvent(QEvent *)
	{

	}

void LabelBotton:: leaveEvent(QEvent *)
	{

	}
