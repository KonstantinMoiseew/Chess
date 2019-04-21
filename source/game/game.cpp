#include "game.h"
#include "iobserver.h"
#include <algorithm>
#include <assert.h>

void Chess::Game::RegisterObserver(IObserver& observer)
{
    if (std::find(observers.begin(), observers.end(), &observer) == observers.end())
        observers.push_back(&observer);
}

void Chess::Game::UnregisterObserver(IObserver& observer)
{
    auto it = std::find(observers.begin(), observers.end(), &observer);
    assert(it != observers.end());
    observers.erase(std::remove(observers.begin(), observers.end(), &observer));
}

const Chess::Observers& Chess::Game::GetObservers() const
{
    return observers;
}

void Chess::Game::AddPiece(Piece& piece)
{
    if (std::find(pieces.begin(), pieces.end(), &piece) == pieces.end())
    {
        pieces.push_back(&piece);
        OBS_CALL(observers, OnPieceAdded(piece));
    }
}

void Chess::Game::RemovePiece(Piece& piece)
{
    auto it = std::find(pieces.begin(), pieces.end(), &piece);
    assert(it != pieces.end());
    pieces.erase(std::remove(pieces.begin(), pieces.end(), &piece));
    OBS_CALL(observers, OnPieceRemoved(piece));
}
