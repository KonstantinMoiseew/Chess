#include "game.h"
#include "iobserver.h"
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

void Chess::Game::AddPiece(Piece& piece)  // Добавляет элемент Piece
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
	piece.SetGame(nullptr);
	OBS_CALL(observers_, OnPieceRemoved(piece));
}

void Chess::Game::ArrangeFigures()
{
	Piece* piece;
	piece = new Chess::Piece(Chess::Type::Knight, Chess::Color::White);
	piece->SetPos({1,0});
	AddPiece(*piece) ;
	piece = new Chess::Piece(Chess::Type::Knight, Chess::Color::White);
	piece->SetPos({6, 0});
	AddPiece(*piece) ;
	piece = new Chess::Piece(Chess::Type::Bishop, Chess::Color::White);
	piece->SetPos({2, 0});
	AddPiece(*piece) ;
	piece = new Chess::Piece(Chess::Type::Bishop, Chess::Color::White);
	piece->SetPos({5, 0});
	AddPiece(*piece) ;
	piece = new Chess::Piece(Chess::Type::Rook, Chess::Color::White);
	piece->SetPos({0, 0});
	AddPiece(*piece) ;
	piece = new Chess::Piece(Chess::Type::Rook, Chess::Color::White);
	piece->SetPos({7, 0});
	AddPiece(*piece) ;
	piece = new Chess::Piece(Chess::Type::Queen, Chess::Color::White);
	piece->SetPos({3, 0});
	AddPiece(*piece) ;
	piece = new Chess::Piece(Chess::Type::King, Chess::Color::White);
	piece->SetPos({4, 0});
	AddPiece(*piece) ;
	for(int i=0; i<8; i++)
	{
		piece = new Chess::Piece(Chess::Type::Pawn, Chess::Color::White);
		piece->SetPos({i, 1});
		AddPiece(*piece) ;
	  }


	piece = new Chess::Piece(Chess::Type::Knight, Chess::Color::Black);
	piece->SetPos({0, 7});
	AddPiece(*piece) ;
	piece = new Chess::Piece(Chess::Type::Knight, Chess::Color::Black);
	piece->SetPos({6, 7});
	AddPiece(*piece) ;
	piece = new Chess::Piece(Chess::Type::Bishop, Chess::Color::Black);
	piece->SetPos({2, 7});
	AddPiece(*piece) ;
	piece = new Chess::Piece(Chess::Type::Bishop, Chess::Color::Black);
	piece->SetPos({5, 7});
	AddPiece(*piece) ;
	piece = new Chess::Piece(Chess::Type::Rook, Chess::Color::Black);
	piece->SetPos({1, 7});
	AddPiece(*piece) ;
	piece = new Chess::Piece(Chess::Type::Rook, Chess::Color::Black);
	piece->SetPos({7, 7});
	AddPiece(*piece) ;
	piece = new Chess::Piece(Chess::Type::Queen, Chess::Color::Black);
	piece->SetPos({3, 7});
	AddPiece(*piece) ;
	piece = new Chess::Piece(Chess::Type::King, Chess::Color::Black);
	piece->SetPos({4, 7});
	AddPiece(*piece) ;
	for(int i=0; i<8; i++)
	{
		piece = new Chess::Piece(Chess::Type::Pawn, Chess::Color::Black);
		piece->SetPos({i, 6});
		AddPiece(*piece) ;
	  }
}

const Chess::Pieces& Chess::Game::GetPieces() const
{
	return pieces_;
}




















