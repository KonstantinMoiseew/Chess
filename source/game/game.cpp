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
	if (std::find_if(pieces_.begin(), pieces_.end(), [&piece](const PieceUnPtr& ptr){return ptr.get() == &piece;}) == pieces_.end()) //Проверяем нет ли в структуре pieces_ такого же элемента
	{
		pieces_.emplace_back(&piece); // Непосредственно добавление
		piece.SetGame(this); // В каждом элементе piece будет храниться указатель на основной объект game_
		OBS_CALL(observers_, OnPieceAdded(piece)); // Проходимся по всем наблюдателям в структуре observers_ и в каждом наблюдателе вызываем функцию OnPieceAdded
		// Для каждого наблюдателя это функция имеет свою реализацию. Причем MainWindow наследуется так же от класса наблюдателя
		// и имеет свою реализацию  OnPieceAdded, а именно создание item (на базе нашего piece) и добавление его на сцену. (Указатель на сцену в объекте  MainWindow).
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
	Piece* piece;
	piece = new Chess::Piece(Chess::Type::Knight, Chess::Color::Black, Chess::Pos(4,4));
	AddPiece(*piece) ;
	piece = new Chess::Piece(Chess::Type::Bishop, Chess::Color::White, Chess::Pos(3,3));
	AddPiece(*piece) ;
	piece = new Chess::Piece(Chess::Type::Rook, Chess::Color::White, Chess::Pos(5,2));
	AddPiece(*piece) ;
	piece = new Chess::Piece(Chess::Type::Queen, Chess::Color::White, Chess::Pos(2,2));
	AddPiece(*piece) ;
	piece = new Chess::Piece(Chess::Type::King, Chess::Color::White, Chess::Pos(7,4));
	AddPiece(*piece) ;
	piece = new Chess::Piece(Chess::Type::Pawn, Chess::Color::White, Chess::Pos(1,1));
	AddPiece(*piece) ;
}

const Chess::Pieces& Chess::Game::GetPieces() const
{
	return pieces_;
}




















