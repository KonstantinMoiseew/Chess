#include "game.h"
#include "iobserver.h"
#include <algorithm>
#include <assert.h>

void Chess::Game::RegisterObserver(IObserver& observer)
{
	if (std::find(observers_.begin(), observers_.end(), &observer) == observers_.end())
		observers_.push_back(&observer);
}

void Chess::Game::UnregisterObserver(IObserver& observer)
{
	auto it = std::find(observers_.begin(), observers_.end(), &observer);
	assert(it != observers_.end());
	observers_.erase(std::remove(observers_.begin(), observers_.end(), &observer));
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
	auto it = std::find_if(pieces_.begin(), pieces_.end(), [&piece](const PieceUnPtr& ptr){return ptr.get() == &piece;});
	assert(it != pieces_.end());
	pieces_.erase(std::remove_if(pieces_.begin(), pieces_.end(), [&piece](const PieceUnPtr& ptr){return ptr.get() == &piece;}));
	OBS_CALL(observers_, OnPieceRemoved(piece));
}

//Chess::Piece  Chess::Game:: FindPiece(Chess::Pos pos_)
//{

//}

void Chess::Game::ArrangeFigures()
{
	Piece* piece;
	//piece = new Chess::Piece(Chess::Type::Knight, Chess::Color::Black, Chess::Pos(4,4));
	//AddPiece(*piece) ;
	//piece = new Chess::Piece(Chess::Type::Bishop, Chess::Color::White, Chess::Pos(3,3));
	//AddPiece(*piece) ;
	piece = new Chess::Piece(Chess::Type::Rook, Chess::Color::White, Chess::Pos(5,2));
	AddPiece(*piece) ;
}


























