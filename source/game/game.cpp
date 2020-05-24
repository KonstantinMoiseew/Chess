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
	auto add_piece = [this](Type type, Color color, Pos pos)
	{
		auto piece = new Chess::Piece(type, color);
		piece->SetPos(pos);
		AddPiece(*piece) ;
	};

	auto add_black_white = [&add_piece](Type type, Pos pos)
	{
		add_piece(type, Color::White, pos);
		add_piece(type, Color::Black, {pos.x_ , 7 - pos.y_});
	};

	add_black_white(Type::Knight, {6, 0});
	add_black_white(Type::Knight, {1, 0});
	add_black_white(Type::Bishop, {5, 0});
	add_black_white(Type::Bishop, {2, 0});
	add_black_white(Type::Rook, {7, 0});
	add_black_white(Type::Rook, {0, 0});
	add_black_white(Type::King, {4, 0});
	add_black_white(Type::Queen, {3, 0});

	for (int i = 0; i < 8; i++)
		add_black_white(Type::Pawn, {i, 1});
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
	//Color::Black ? Color::White : Color::Black

}

bool Chess::Game::IsKingAttacked(Color color) const
{
	auto it_king = std::find_if(pieces_.begin(), pieces_.end(), [color](auto& piece) {return piece-> GetColor() == color && piece-> GetType() == Type::King;});
	if (it_king == pieces_.end())
		return false;

	for (auto& piece :  pieces_)
	{
		if(piece->GetColor()!=color)
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

bool Chess::Game:: HasKingAttackedAfterMove(Color color) const
{
	auto it_king = std::find_if(pieces_.begin(), pieces_.end(), [color](auto& piece) {return piece-> GetColor() != color && piece-> GetType() == Type::King;});
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






