#include "commands.h"
#include "game.h"

std::vector<char> Chess::ICommand::Serialize(ICommand* command)
{
	return command->Write();
}

Chess::ICommand* Chess::ICommand::Deserialize(const std::vector<char>& data)
{
	auto type = (Type)data.front();
	ICommand* cmd = nullptr;
	if (type == Type::Move)
		cmd = new MoveCommand;
	//else if (type == Type::Promote)
		//cmd = new ...

	if (cmd)
		cmd->Read(data);

	return cmd;
}

Chess::MoveCommand::MoveCommand()
{
}

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

bool Chess::MoveCommand::Validate(const Game& game, bool flag_changable) const
{
	if(!flag_changable)
	{
		auto piece = game.FindPieceAt(posFrom_);
		assert(piece);
		if (!piece)
			return false;

		if ((game.GetPlayerTurn() != piece->GetColor()))
			return false;

		auto available_movement = piece->GetMovement().GetAvailableMovement();
		if (std::find(available_movement.begin(), available_movement.end(), posTo_) == available_movement.end())
			return false;

		if (posFrom_ == posTo_)
			return false;

		auto piece_enemy = game.FindPieceAt(posTo_);
		if (piece_enemy && piece_enemy->GetColor() == piece->GetColor())
			return false;

		if (std::find_if(game.GetPieces().begin(), game.GetPieces().end(), [](const PieceUnPtr& ptr){return (ptr.get()->GetType() == Chess::Type::Pawn&&(ptr.get()->GetPos().y_==0||ptr.get()->GetPos().y_==7));}) != game.GetPieces().end())
			return false;
	}

	return true;
}

void Chess::MoveCommand::Do(Game& game, bool flag_changable)
{
	flagChangable=flag_changable;
	if(!flag_changable)
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
		pieceColor_=piece->GetColor();
		piece->SetPos(posTo_);
		pieceHasMovedBefore_ = piece->HasMoved();
		piece->SetHasMoved(true);
	}
	else
	{
		auto piece2 = game.FindPieceAt(posTo_);
		assert(piece2);
		if (!piece2)
			return;

		pieceHasMovedBefore_ =piece2->HasMoved();
		enemyPiece_ = piece2->Serialize();//сохраняем пешку как врага
		game.RemovePiece(*piece2);//удаляем пешку

		auto changedPiece = new Piece(Chess::Type::Queen, Chess::Color::White);
		game.AddPiece(*changedPiece);
		pieceType_= changedPiece->GetType();//записываем
		pieceColor_=changedPiece->GetColor();
		changedPiece->SetHasMoved(true);
		changedPiece->SetPos(posTo_);
	}
}

void Chess::MoveCommand::Undo(Game& game, bool flag_changable)
{
	if(!flag_changable)
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
		game.AddPiece(*enemy);
		enemy->SetPos(posTo_);
		}
	}

	else
	{
	{
	auto piece = game.FindPieceAt(posFrom_);
	game.RemovePiece(*piece);
	if (enemyPiece_)
	{
		auto enemy = new Piece(*enemyPiece_);
		enemy->SetHasMoved(pieceHasMovedBefore_);
		game.AddPiece(*enemy);
		enemy->SetPos(posTo_);
	}

	}
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

Chess::Type Chess::MoveCommand::GetPieceType() const
{
	return pieceType_;
}

Chess::Color Chess::MoveCommand::GetPieceColor() const
{
	return pieceColor_;
}

bool Chess::MoveCommand::DidCapture() const
{
	return enemyPiece_.has_value();
}

Chess::Type Chess::MoveCommand::GetCapturedPieceType() const
{
	return enemyPiece_ ? enemyPiece_.value().type_ : Type::Pawn;
}

Chess::Color Chess::MoveCommand::GetCapturedPieceColor() const
{
	return enemyPiece_ ? enemyPiece_.value().color_ : Color::White;
}

bool Chess::MoveCommand::CheckEmptyEnemy() const
{
	return enemyPiece_.has_value();
}

bool Chess::MoveCommand::KingUnderAttak() const
{
	return kingUnderAtak_;
}

std::vector<char> Chess::MoveCommand::Write() const
{

}

void Chess::MoveCommand::Read(const std::vector<char>& data)
{

}











