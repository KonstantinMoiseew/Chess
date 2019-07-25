#include "piecemovements.h"
#include "source/game/defs.h"
#include "piece.h"
#include <algorithm>

Chess::RookMovement::RookMovement(Piece& owner)
	: piece_(owner)
{	
}

Chess::Positions Chess::RookMovement::GetAvailableMovement() const
{
	Positions result;
	for (int i = 0; i < BoardSize; i++)
	{
		result.push_back(Pos(piece_.GetPos().x_, i));
	}

	for (int i = 0; i < BoardSize; i++)
	{
		result.push_back(Pos(i, piece_.GetPos().y_));
	}

	return result;
}

Chess::BishopMovement::BishopMovement(Piece& owner)
	: piece_(owner)
{
}

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






