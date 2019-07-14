#include "iobserver.h"
#include "game.h"

Chess::IObserver::~IObserver()
{
	if (game_)
		game_->UnregisterObserver(*this);
}

void Chess::IObserver::OnRegistered(Game& game)
{
	game_ = &game;
}

void Chess::IObserver::OnUnregistered()
{
	game_ = nullptr;
}
