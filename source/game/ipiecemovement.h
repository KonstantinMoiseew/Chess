#pragma once

#include "position.h"

namespace Chess
{
	class IPieceMovement
	{
	public:

		virtual ~IPieceMovement() {}

		virtual Positions GetAvailableMovement() const = 0;
	};
}
