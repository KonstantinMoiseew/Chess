#include "commands.h"
#include "game.h"

Chess::MoveCommand::MoveCommand(Piece& piece, const Pos& pos)
{
	posFrom_ = piece.GetPos();
	posTo_ = pos;
}

Chess::MoveCommand::MoveCommand(const Pos& pos_from, const Pos& pos_to)
{
	posFrom_ = pos_from;
	posTo_ = pos_to;
}

bool Chess::MoveCommand::Validate(const Game& game) const
{
	auto piece = game.FindPieceAt(posFrom_);
	assert(piece);
	if (!piece)
		return false;

	if (game.GetPlayerTurn() != piece->GetColor())
		return false;

	auto available_movement = piece->GetMovement().GetAvailableMovement();
	if (std::find(available_movement.begin(), available_movement.end(), posTo_) == available_movement.end())
		return false;

	if (posFrom_ == posTo_)
		return false;

	auto piece_enemy = game.FindPieceAt(posTo_);
	if (piece_enemy && piece_enemy->GetColor() == piece->GetColor())
		return false;

	return true;
}

void Chess::MoveCommand::Do(Game& game)
{
	auto piece = game.FindPieceAt(posFrom_);
	assert(piece);
	if (!piece)
		return;

	if (auto enemy = game.FindPieceAt(posTo_))
	{
		enemyPiece_ = enemy->Serialize();
		game.RemovePiece(*enemy);
	}
	pieceType_= piece->GetType();
	pieceColore_=piece->GetColor();
	piece->SetPos(posTo_);
	pieceHasMovedBefore_ = piece->HasMoved();
	piece->SetHasMoved(true);
}

void Chess::MoveCommand::Undo(Game& game)
{
	auto piece = game.FindPieceAt(posTo_);
	assert(piece);
	if (!piece)
		return;

	piece->SetPos(posFrom_);
	piece->SetHasMoved(pieceHasMovedBefore_);

	if (enemyPiece_)
	{
		auto enemy = new Piece(*enemyPiece_);
		enemy->SetPos(posTo_);
		game.AddPiece(*enemy);
	}
}

std::string Chess::MoveCommand::ToString() const
{
	if (pieceType_ == Type::Pawn)
		return posFrom_.ToString() + "->" + posTo_.ToString();
	else if (pieceType_ ==Type::Knight)
		return "N" + posFrom_.ToString() + "->" + posTo_.ToString();
	else if (pieceType_ ==Type::Bishop)
		return "B" +posFrom_.ToString() + "->" + posTo_.ToString();
	else if (pieceType_ ==Type::Rook)
		return "R" +  posFrom_.ToString() + "->" +posTo_.ToString();
	else if (pieceType_ ==Type::Queen)
		return "Q" + posFrom_.ToString() + "->" +posTo_.ToString();
	else if (pieceType_ ==Type::King)
		return "K" + posFrom_.ToString() + "->" +posTo_.ToString();

	return "";
}


QPixmap Chess::MoveCommand::ToPix() const
{
		if(pieceColore_==Color::Black){
	if (pieceType_ == Type::Pawn){
		return QPixmap (":/graphics/pieces/pawn_black.png").scaledToWidth(30,  Qt::FastTransformation);
	}
	else if (pieceType_ ==Type::Knight)
		return QPixmap (":/graphics/pieces/knight_black.png").scaledToWidth(30,  Qt::FastTransformation);
	else if (pieceType_ ==Type::Bishop)
		return QPixmap (":/graphics/pieces/bishop_black.png").scaledToWidth(30,  Qt::FastTransformation);
	else if (pieceType_ ==Type::Rook)
		return QPixmap (":/graphics/pieces/rook_black.png").scaledToWidth(30,  Qt::FastTransformation);
	else if (pieceType_ ==Type::Queen)
		return QPixmap (":/graphics/pieces/queen_black.png").scaledToWidth(30,  Qt::FastTransformation);
	else if (pieceType_ ==Type::King)
		return QPixmap (":/graphics/pieces/king_black.png").scaledToWidth(30,  Qt::FastTransformation);
		}
		else {
			if (pieceType_ == Type::Pawn)
				return QPixmap (":/graphics/pieces/pawn_white.png").scaledToWidth(30,  Qt::FastTransformation);
			else if (pieceType_ ==Type::Knight)
				return QPixmap (":/graphics/pieces/knight_white.png").scaledToWidth(30,  Qt::FastTransformation);
			else if (pieceType_ ==Type::Bishop)
				return QPixmap (":/graphics/pieces/bishop_white.png").scaledToWidth(30,  Qt::FastTransformation);
			else if (pieceType_ ==Type::Rook)
				return QPixmap (":/graphics/pieces/rook_white.png").scaledToWidth(30,  Qt::FastTransformation);
			else if (pieceType_ ==Type::Queen)
				return QPixmap (":/graphics/pieces/queen_white.png").scaledToWidth(30,  Qt::FastTransformation);
			else if (pieceType_ ==Type::King)
				return QPixmap (":/graphics/pieces/king_white.png").scaledToWidth(30,  Qt::FastTransformation);
		}

	return QPixmap ("").scaledToWidth(30,  Qt::FastTransformation);
}

















