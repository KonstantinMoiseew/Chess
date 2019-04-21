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

        const Observers& GetObservers() const;

        void AddPiece(Piece&);
        void RemovePiece(Piece&);

    protected:

        Observers observers;
        Pieces pieces;
    };

}

