#pragma once
#include <QGraphicsItem>
#include <QObject>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QCursor>
#include "game/iobserver.h"
#include "game/game.h"


class PieceItem:   public Chess::IObserver, public QPixmap
{
public:
	PieceItem(Chess::Game * game, const QString& fileName, const char *format = nullptr, Qt::ImageConversionFlags flags = Qt::AutoColor): QPixmap(fileName, format, flags)
	{

		game-> RegisterObserver(*this); //registration

		ptr_game=game;

	}
	void OnPieceAdded(Chess::Piece&) override
	{

	}
	void OnPieceRemoved(Chess::Piece&) override
	{

	}

	~ PieceItem() override
	{

	}


private:
	Chess::Game * ptr_game;

};
