#pragma once

#include "ipiecemovement.h"
#include "gamecommon.h"
#include <memory>

namespace Chess
{
	class RookMovement : public IPieceMovement
	{
	public:

		RookMovement(Piece& owner);
        Positions GetAvailableMovement(bool attack_only = false) const override;

	protected:

		Piece& piece_;
	};

	class BishopMovement : public IPieceMovement
	{
	public:

		BishopMovement(Piece& owner);
        Positions GetAvailableMovement(bool attack_only = false) const override;

	protected:

		Piece& piece_;
	};

	class QueenMovement : public IPieceMovement
	{
	public:

		QueenMovement(Piece& owner);
        Positions GetAvailableMovement(bool attack_only = false) const override;

	protected:

		std::unique_ptr<RookMovement> rookMovement_;
		std::unique_ptr<BishopMovement> bishopMovement_;
	};

	class KingMovement : public IPieceMovement
	{
	public:

		KingMovement(Piece& owner);
        Positions GetAvailableMovement(bool attack_only = false) const override;

	protected:

		Piece& piece_;
		std::unique_ptr<QueenMovement> queenMovement_;
	};

	class KnightMovement : public IPieceMovement
	{
	public:

		KnightMovement(Piece& owner);
        Positions GetAvailableMovement(bool attack_only = false) const override;

	protected:

		Piece& piece_;
	};

	class PawnMovement : public IPieceMovement
	{
	public:

		PawnMovement(Piece& owner);
        Positions GetAvailableMovement(bool attack_only = false) const override;

	protected:

		Piece& piece_;
	};


}




















