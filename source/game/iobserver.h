#pragma once

#include "gamecommon.h"
#include <vector>

#define OBS_CALL(observers, event) \
	auto observers_copy = observers; \
	for (auto& obs : observers_copy) \
        obs->event;

namespace Chess
{

    class IObserver
    {
    public:

		virtual ~IObserver();

		virtual void OnRegistered(Game&);
		virtual void OnUnregistered();
        virtual void OnPieceAdded(Piece&) {}
		virtual void OnPieceAboutToBeRemoved(Piece&) {}
		virtual void OnPieceMoved(Piece&) {}

	protected:

		Game* game_ = nullptr;
    };

	using Observers = std::vector<IObserver*>;

}
