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
		//OBS_CALL(observers_, OnPieceAdded(piece));
		observers_[0]->OnPieceAdded(piece);
	}
}

void Chess::Game::RemovePiece(Piece& piece)
{
	auto it = std::find_if(pieces_.begin(), pieces_.end(), [&piece](const PieceUnPtr& ptr){return ptr.get() == &piece;});
	assert(it != pieces_.end());
	pieces_.erase(std::remove_if(pieces_.begin(), pieces_.end(), [&piece](const PieceUnPtr& ptr){return ptr.get() == &piece;}));
	OBS_CALL(observers_, OnPieceRemoved(piece));
}

void Chess::Game::ArrangeFigures()
{
	Piece * Knight_Black = new Chess::Piece(Chess::Type::Knight, Chess::Color::Black, Chess::Pos(1,0));
	this->AddPiece(*Knight_Black) ;
}


























