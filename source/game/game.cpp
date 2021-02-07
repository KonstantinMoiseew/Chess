#include "game.h"
#include "iobserver.h"
#include "commands.h"
#include <algorithm>
#include <assert.h>

Chess::Game::~Game()
{
	auto obs_copy = observers_;
	for (auto& observer : obs_copy)
		UnregisterObserver(*observer);
}

void Chess::Game::RegisterObserver(IObserver& observer)
{
	if (std::find(observers_.begin(), observers_.end(), &observer) == observers_.end())
	{
		observers_.push_back(&observer);
		observer.OnRegistered(*this);
	}
}

void Chess::Game::UnregisterObserver(IObserver& observer)
{
	auto it = std::find(observers_.begin(), observers_.end(), &observer);
	assert(it != observers_.end());
	observers_.erase(std::remove(observers_.begin(), observers_.end(), &observer));
	observer.OnUnregistered();
}

const Chess::Observers& Chess::Game::GetObservers() const
{
	return observers_;
}

void Chess::Game::AddPiece(Piece& piece)
{
	if (std::find_if(pieces_.begin(), pieces_.end(), [&piece](const PieceUnPtr& ptr){return ptr.get() == &piece;}) == pieces_.end())
	{
		pieces_.emplace_back(&piece);
		piece.SetGame(this);
		OBS_CALL(observers_, OnPieceAdded(piece));
	}
}

void Chess::Game::RemovePiece(Piece& piece)
{
	OBS_CALL(observers_, OnPieceAboutToBeRemoved(piece));
	auto it = std::find_if(pieces_.begin(), pieces_.end(), [&piece](const PieceUnPtr& ptr){return ptr.get() == &piece;});
	assert(it != pieces_.end());
	pieces_.erase(std::remove_if(pieces_.begin(), pieces_.end(), [&piece](const PieceUnPtr& ptr){return ptr.get() == &piece;}));
}

void Chess::Game:: RemoveAllPieces()
{
	for ( const auto & elem : pieces_ )
   {
	   OBS_CALL(observers_, OnPieceAboutToBeRemoved(*(elem.get())));
   }
	pieces_.clear();
	activePlayer_ = Color::White;

}

void Chess::Game::ArrangeFigures()
{
    auto add_piece = [this](PieceType type, Color color, Pos pos)
	{
		auto piece = new Chess::Piece(type, color);
		piece->SetPos(pos);
		AddPiece(*piece) ;
	};

    auto add_black_white = [&add_piece](PieceType type, Pos pos)
	{
		add_piece(type, Color::White, pos);
		add_piece(type, Color::Black, {pos.x_ , 7 - pos.y_});
	};

    add_black_white(PieceType::Knight, {6, 0});
    add_black_white(PieceType::Knight, {1, 0});
    add_black_white(PieceType::Bishop, {5, 0});
    add_black_white(PieceType::Bishop, {2, 0});
    add_black_white(PieceType::Rook, {7, 0});
    add_black_white(PieceType::Rook, {0, 0});
    add_black_white(PieceType::King, {4, 0});
    add_black_white(PieceType::Queen, {3, 0});

    for (int i = 0; i < Chess::BoardSize; i++)
        add_black_white(PieceType::Pawn, {i, 1});
}

const Chess::Pieces& Chess::Game::GetPieces() const
{
	return pieces_;
}

Chess::Piece* Chess::Game::FindPieceAt(const Pos& pos) const
{
	auto it = std::find_if(pieces_.begin(), pieces_.end(), [pos](auto& other_piece) {return other_piece->GetPos() == pos;});
	if (it != pieces_.end())
		return it->get();

	return nullptr;
}

void Chess::Game::SetPlayerTurn(Color color)
{
	activePlayer_ = color;
}

Chess::Color Chess::Game::GetPlayerTurn() const
{
	return activePlayer_;
}

void Chess::Game::NextPlayerTurn()
{
	activePlayer_ = activePlayer_ == Color::White ? Color::Black : Color::White;
	if (IsCheckMate(activePlayer_))
	{
		OBS_CALL(observers_, OnGameOver(activePlayer_ == Color::White ? Color::Black : Color::White));
	}
}

bool Chess::Game::IsCellAttacked(Pos pos, Color attackers_color) const
{
    for (auto& piece :  pieces_)
    {
        if(piece->GetColor() == attackers_color)
        {
            for (auto mov : piece->GetMovement().GetAvailableMovement(true))
                if (pos == mov)
                    return true;
        }
    }
    return false;
}

bool Chess::Game::IsKingAttacked(Color color) const
{
    auto it_king = std::find_if(pieces_.begin(), pieces_.end(), [color](auto& piece) {return piece-> GetColor() == color && piece-> GetType() == PieceType::King;});
	if (it_king == pieces_.end())
		return false;

    return IsCellAttacked((*it_king)->GetPos(), GetOppositeColor(color));
}

bool Chess::Game::IsCheckMate(Color color) const
{
	std::vector<bool> king_attacked_massive;
	if(!IsKingAttacked(color))
		return false;

	for (auto& piece :  pieces_)
	{
		if(piece->GetColor()==color)
		{
			for (auto& pos :  piece->GetMovement().GetAvailableMovement())
			{
				MoveCommand move(*piece, pos);
				auto& game = *const_cast<Game*>(this);//this (*Game) - const
				move.Do(game);
				bool king_attacked = IsKingAttacked(color);
				move.Undo(game);
				if (!king_attacked)
					return false;
			}
		}
	}
	 return true;
}

bool Chess::Game::HasKingAttackedAfterMove(Color color) const
{
    auto it_king = std::find_if(pieces_.begin(), pieces_.end(), [color](auto& piece) {return piece->GetColor() != color && piece-> GetType() == PieceType::King;});
	if (it_king == pieces_.end())
		return false;

	for (auto& piece :  pieces_)
	{
		if(piece->GetColor()==color)
		{
			for (auto mov : piece->GetMovement().GetAvailableMovement())
				if ((*it_king)->GetPos() == mov)
				{
					return true;
				}
		}
	}
	return false;
}

Chess::ICommand* Chess::Game::CreateCommand(Piece& piece, Pos pos) const
{
    auto move_delta = pos - piece.GetPos();
    if (piece.GetType() == Chess::PieceType::King &&
        abs(move_delta.x_) == 2)
    {
        std::vector<Chess::ICommandUnPtr> commands;
        commands.push_back(Chess::ICommandUnPtr(new Chess::MoveCommand(piece, pos)));
        Piece* rook = nullptr;
        if (move_delta.x_ > 0)
            rook = FindPieceAt({7, pos.y_});
        else
            rook = FindPieceAt({0, pos.y_});

        if (!rook)
            return nullptr;

        commands.push_back(Chess::ICommandUnPtr(new Chess::MoveCommand(*rook, pos - Chess::Pos(1, 0) * sgn(move_delta.x_))));
        return new Chess::CompoundCommand(std::move(commands));
    }

    return new Chess::MoveCommand(piece, pos);
}
