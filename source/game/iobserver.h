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

        virtual ~IObserver() {}

        virtual void OnPieceAdded(Piece&) {}
        virtual void OnPieceRemoved(Piece&) {}
    };

	using Observers = std::vector<IObserver*>;

}
