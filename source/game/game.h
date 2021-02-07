#pragma once

#include "gamecommon.h"
#include "iobserver.h"
#include "piece.h"

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

        bool IsCellAttacked(Pos pos, Color attackers_color) const;
		bool IsKingAttacked(Color color) const;
		bool IsCheckMate(Color color) const;
		bool HasKingAttackedAfterMove(Color color) const;

        ICommand* CreateCommand(Piece& piece, Pos pos) const;

    protected:

		Observers observers_;
		Pieces pieces_;
		Color activePlayer_ = Color::White;
    };

}

