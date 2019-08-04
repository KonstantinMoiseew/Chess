#include "piecemovements.h"
#include "game/defs.h"
#include "piece.h"
#include "game/game.h"
#include <algorithm>

Chess::RookMovement::RookMovement(Piece& owner)
	: piece_(owner)
{}

Chess::Positions Chess::RookMovement::GetAvailableMovement() const
{
	Positions result;

	auto pos = piece_.GetPos();

	for (int i = pos.y_; i < BoardSize; i++)
	{
		if (!AddPos(Pos(pos.x_, i), result))
			break;
	}

	for (int i = pos.y_; i >= 0; i--)
	{
		if (!AddPos(Pos(pos.x_, i), result))
			break;
	}

	for (int i = pos.x_; i < BoardSize; i++)
	{
		if (!AddPos(Pos(i, pos.y_), result))
			break;
	}

	for (int i = pos.x_; i >= 0; i--)
	{
		if (!AddPos(Pos(i, pos.y_), result))
			break;
	}

	return result;
}

bool Chess::RookMovement::AddPos(const Pos& pos, Positions& result) const
{
	auto& pieces = piece_.GetGame()->GetPieces();
	auto it = std::find_if(pieces.begin(), pieces.end(), [this, pos](auto& piece){return piece.get() != &piece_ && piece->GetPos() == pos;});
	if (it != pieces.end())
	{
		if ((*it)->GetColor() != piece_.GetColor())
			result.push_back(pos);

		return false;
	}

	result.push_back(pos);
	return true;
}

Chess::BishopMovement::BishopMovement(Piece& owner)
	: piece_(owner)
{}

Chess::Positions Chess::BishopMovement::GetAvailableMovement() const
{
	Positions result;

	for (int i =-BoardSize; i < BoardSize; i++)
	{
		Pos pos(piece_.GetPos().x_ + i, piece_.GetPos().y_ + i);
		if (pos.IsValid())
			result.push_back(pos);
	}

	for (int i =-BoardSize; i < BoardSize; i++)
	{
		Pos pos(piece_.GetPos().x_ - i, piece_.GetPos().y_ + i);
		if (pos.IsValid())
			result.push_back(pos);
	}

	return result;
}

Chess::KnightMovement::KnightMovement(Piece& owner)
	:piece_(owner)
{}

Chess::Positions Chess::KnightMovement:: GetAvailableMovement() const
{
	Positions result;
	result.push_back(Pos(piece_.GetPos().x_+1,piece_.GetPos().y_+2));
	result.push_back(Pos(piece_.GetPos().x_+1,piece_.GetPos().y_-2));
	result.push_back(Pos(piece_.GetPos().x_-1,piece_.GetPos().y_+2));
	result.push_back(Pos(piece_.GetPos().x_-1,piece_.GetPos().y_-2));
	result.push_back(Pos(piece_.GetPos().x_+2,piece_.GetPos().y_+1));
	result.push_back(Pos(piece_.GetPos().x_+2,piece_.GetPos().y_-1));
	result.push_back(Pos(piece_.GetPos().x_-2,piece_.GetPos().y_+1));
	result.push_back(Pos(piece_.GetPos().x_-2,piece_.GetPos().y_-1));
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

	for (int i =0; i <= movement; i++)
	{
		Pos pos(piece_.GetPos().x_ , piece_.GetPos().y_ + i);
		if (pos.IsValid())
			result.push_back(pos);
	}

	return result;
}








