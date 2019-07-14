#include "piecemovements.h"
#include "source/game/defs.h"
#include "piece.h"

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












