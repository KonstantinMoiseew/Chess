#pragma once

#include "ipiecemovement.h"
#include "gamecommon.h"

namespace Chess
{
	class RookMovement : public IPieceMovement
	{
	public:

		RookMovement(Piece& owner);
		Positions GetAvailableMovement() const override;

	protected:
		Piece& piece_;
	};

	class BishopMovement : public IPieceMovement
	{
	public:

		BishopMovement(Piece& owner);
		Positions GetAvailableMovement() const override;

	protected:
		Piece& piece_;
	};

	class QueenMovement : public IPieceMovement
	{
	public:

		QueenMovement(Piece& owner);
		Positions GetAvailableMovement() const override;

	protected:
		Piece& piece_;
	};

	class KingMovement : public IPieceMovement
	{
	public:

		KingMovement(Piece& owner);
		Positions GetAvailableMovement() const override;

	protected:
		Piece& piece_;
	};

	class KnightMovement : public IPieceMovement
	{
	public:

		KnightMovement(Piece& owner);
		Positions GetAvailableMovement() const override;

	protected:
		Piece& piece_;
	};


}
