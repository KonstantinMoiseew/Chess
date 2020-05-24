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
		void RemoveAllPieces();

		const Pieces& GetPieces() const;

		Piece* FindPieceAt(const Pos& pos) const;

		void SetPlayerTurn(Color color);
		Color GetPlayerTurn() const;
		void NextPlayerTurn();

		bool IsKingAttacked(Color color) const;
		bool IsCheckMate(Color color) const;
		bool HasKingAttackedAfterMove(Color color) const;

    protected:

		Observers observers_;
		Pieces pieces_;
		Color activePlayer_ = Color::White;

    };

}

