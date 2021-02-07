#pragma once

#include "position.h"

namespace Chess
{
	class IPieceMovement
	{
	public:

		virtual ~IPieceMovement() {}

        virtual Positions GetAvailableMovement(bool attack_only = false) const = 0;
	};
}
