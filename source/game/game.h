#pragma once

#include "gamecommon.h"
#include "iobserver.h"
#include "piece.h"

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
		//Piece FindPiece(Pos pos_);

    protected:

		Observers observers_; //holding observers-PieceItem which on the scene
		Pieces pieces_; // holding logical figures which don't relate to PieceItem.     using PieceUnPtr = std::unique_ptr<Piece>; using Pieces = std::vector<PieceUnPtr>;
    };

}

