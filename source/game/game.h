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

		virtual ~Game();

        void RegisterObserver(IObserver&);
        void UnregisterObserver(IObserver&);
		const Observers& GetObservers() const;

		void ArrangeFigures( );

        void AddPiece(Piece&);
        void RemovePiece(Piece&);

		const Pieces& GetPieces() const;

    protected:

		Observers observers_; //holding observers-PieceItem which on the scene
		Pieces pieces_; // holding logical figures which don't relate to PieceItem.     using PieceUnPtr = std::unique_ptr<Piece>; using Pieces = std::vector<PieceUnPtr>;
    };

}

