#pragma once

#include "gamecommon.h"
#include "iobserver.h"
#include "piece.h"
#include <iostream>
#include <QDir>
#include <QStringList>
#include <QDebug>
#include <QTimer>
#include <QRandomGenerator>
namespace Chess
{

    class Game
    {
    public:

        void RegisterObserver(IObserver&);
        void UnregisterObserver(IObserver&);
		void ArrangeFigures( );

        const Observers& GetObservers() const;

        void AddPiece(Piece&);
        void RemovePiece(Piece&);


		void MovingPiece()
		{

			int x_=(qrand() % ((7 + 1) - 0) + 0);
			int y_=(qrand() % ((7 + 1) - 0) + 0);
			Pos pos (x_,y_);
			pieces_.at(1)->SetPos(pos);

		}

    protected:

		Observers observers_; //holding observers-PieceItem which on the scene
		Pieces pieces_; // holding logical figures which don't relate to PieceItem.     using PieceUnPtr = std::unique_ptr<Piece>; using Pieces = std::vector<PieceUnPtr>;
    };

}

