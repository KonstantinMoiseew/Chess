#include "piecemovements.h"
#include "game/defs.h"
#include "piece.h"
#include "game/game.h"
#include <algorithm>

static bool AddPos(const Chess::Piece& piece, const Chess::Pos& pos, Chess::Positions& result, bool allow_opposite_color = true, bool allow_empty_cell = true)
{
	if (pos == piece.GetPos())
		return true;

	if (piece.GetGame())
	{
		auto& pieces = piece.GetGame()->GetPieces();
		auto it = std::find_if(pieces.begin(), pieces.end(), [&piece, pos](auto& other_piece){return other_piece.get() != &piece && other_piece->GetPos() == pos;});
		if (it != pieces.end())
		{
			if ((*it)->GetColor() != piece.GetColor() && allow_opposite_color)
				result.push_back(pos);

			return false;
		}
	}

	if (allow_empty_cell)
	{
		result.push_back(pos);
		return true;
	}

	return false;
}

Chess::RookMovement::RookMovement(Piece& owner)
	: piece_(owner)
{}

Chess::Positions Chess::RookMovement::GetAvailableMovement() const
{
	Positions result;
	auto pos = piece_.GetPos();

	for (int i = pos.y_; i < BoardSize; i++)
	{
		if (!AddPos(piece_, Pos(pos.x_, i), result))
			break;
	}

	for (int i = pos.y_; i >= 0; i--)
	{
		if (!AddPos(piece_, Pos(pos.x_, i), result))
			break;
	}

	for (int i = pos.x_; i < BoardSize; i++)
	{
		if (!AddPos(piece_, Pos(i, pos.y_), result))
			break;
	}

	for (int i = pos.x_; i >= 0; i--)
	{
		if (!AddPos(piece_, Pos(i, pos.y_), result))
			break;
	}

	return result;
}

Chess::BishopMovement::BishopMovement(Piece& owner)
	: piece_(owner)
{}

Chess::Positions Chess::BishopMovement::GetAvailableMovement() const
{
	auto pos = piece_.GetPos();
	Positions result;

	for (int i = 0;; i++)
	{
		auto new_pos = pos + Pos(i, i);
		if (!new_pos.IsValid())
			break;

		if (!AddPos(piece_, new_pos, result))
			break;
	}

	for (int i = 0;; i++)
	{
		auto new_pos = pos + Pos(-i, i);
		if (!new_pos.IsValid())
			break;

		if (!AddPos(piece_, new_pos, result))
			break;
	}

	for (int i = 0;; i++)
	{
		auto new_pos = pos + Pos(-i, -i);
		if (!new_pos.IsValid())
			break;

		if (!AddPos(piece_, new_pos, result))
			break;
	}

	for (int i = 0;; i++)
	{
		auto new_pos = pos + Pos(i, -i);
		if (!new_pos.IsValid())
			break;

		if (!AddPos(piece_, new_pos, result))
			break;
	}

	return result;
}

Chess::KnightMovement::KnightMovement(Piece& owner)
	:piece_(owner)
{}

Chess::Positions Chess::KnightMovement:: GetAvailableMovement() const
{
	Positions result;
	AddPos(piece_, piece_.GetPos() + Pos(+1,+2), result);
	AddPos(piece_, piece_.GetPos() + Pos(+1,-2), result);
	AddPos(piece_, piece_.GetPos() + Pos(-1,+2), result);
	AddPos(piece_, piece_.GetPos() + Pos(-1,-2), result);
	AddPos(piece_, piece_.GetPos() + Pos(+2,+1), result);
	AddPos(piece_, piece_.GetPos() + Pos(+2,-1), result);
	AddPos(piece_, piece_.GetPos() + Pos(-2,+1), result);
	AddPos(piece_, piece_.GetPos() + Pos(-2,-1), result);
	result.erase(std::remove_if(result.begin(), result.end(), [](auto& p) {return !p.IsValid();} ), result.end());

	return result;
}

Chess::QueenMovement::QueenMovement(Piece& owner)
{
	rookMovement_.reset(new RookMovement(owner));
	bishopMovement_.reset(new BishopMovement(owner));
}

Chess::Positions Chess::QueenMovement::GetAvailableMovement() const
{
	Positions resultR = rookMovement_->GetAvailableMovement();
	Positions resultB = bishopMovement_->GetAvailableMovement();
	resultR.insert(resultR.end(), resultB.begin(), resultB.end());
	return resultR;
}

Chess::KingMovement::KingMovement(Piece& owner)
	: piece_(owner)
{
	queenMovement_.reset(new QueenMovement(owner));
}

Chess::Positions Chess::KingMovement::GetAvailableMovement() const
{
	Positions result  = queenMovement_->GetAvailableMovement();
	result.erase(std::remove_if(result.begin(), result.end(), [this](auto& p) {return (p - piece_.GetPos()).LengthSqr() >= 4;} ), result.end());
	return result;
}


Chess::PawnMovement::PawnMovement(Piece& owner)
	: piece_(owner)
{}

Chess::Positions Chess::PawnMovement::GetAvailableMovement() const
{
	Positions result;

	int movement = piece_.HasMoved() ? 1 : 2;

	int color_factor = piece_.GetColor() == Color::White ? 1 : -1;

	for (int i =0; i <= movement; i++)
	{
		auto pos = piece_.GetPos() + Pos(0, i * color_factor);
		if (pos.IsValid())
			if (!AddPos(piece_, pos, result, false))
				break;
	}

	{
		auto pos = piece_.GetPos() + Pos(1, color_factor);
		if (pos.IsValid())
			AddPos(piece_, pos, result, true, false);
	}

	{
		auto pos = piece_.GetPos() + Pos(-1, color_factor);
		if (pos.IsValid())
			AddPos(piece_, pos, result, true, false);
	}

	return result;
}








