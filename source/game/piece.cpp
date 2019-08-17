#include "piece.h"
#include "defs.h"
#include "gamecommon.h"
#include "game.h"
#include "iobserver.h"
#include "piecemovements.h"

Chess::Piece::Piece(Type type, Color color)
{
	color_ = color;
	type_ = type;

	switch(type)
	{
	case Type::Rook: movement_.reset(new RookMovement(*this)); break;
	case Type::Bishop: movement_.reset(new BishopMovement(*this)); break;
	case Type::Knight: movement_.reset(new KnightMovement(*this)); break;
	case Type::Queen: movement_.reset(new QueenMovement(*this)); break;
	case Type::King: movement_.reset(new KingMovement(*this)); break;
	case Type::Pawn: movement_.reset(new PawnMovement(*this)); break;
	}
}

Chess:: Color Chess::Piece:: GetColor() const
{
	return color_;
}

Chess::Type Chess::Piece:: GetType() const
{
	return type_;
}

Chess::Pos Chess::Piece:: GetPos() const
{
	return position_;
}

Chess::Game* Chess::Piece::GetGame() const
{
	return game_;
}

void Chess::Piece::SetGame(Game* game)
{
	game_ = game;
}

Chess::IPieceMovement& Chess::Piece::GetMovement()
{
	return *movement_;
}

void Chess::Piece:: SetPos(const Pos& pos)
{
	position_ = pos;
	if (game_)
	{
		OBS_CALL(game_->GetObservers(), OnPieceMoved(*this));
	}
}

void Chess::Piece:: MoveToPos(const Pos& pos)
{
	auto& pieces = this->GetGame()->GetPieces();
	auto it = std::find_if(pieces.begin(), pieces.end(), [pos](auto& other_piece){return other_piece->GetPos() == pos;});
	if (it != pieces.end())
	{
		if ((*it)->GetColor() != this->GetColor())
			this->GetGame()->RemovePiece(*(it->get()));
	}

	auto available_movement = movement_->GetAvailableMovement();
	if (std::find(available_movement.begin(), available_movement.end(), pos) != available_movement.end())
	{
		if (position_!=pos)
		{hasMoved_ = true;}
		position_ = pos;
	} // position_  is changed only for suitable moves
	OBS_CALL(game_->GetObservers(), OnPieceMoved(*this));
}

void Chess::Piece::ResetMovementFlag()
{
	hasMoved_ = false;
}

bool Chess::Piece:: HasMoved() const
{
	return hasMoved_;
}








