#pragma once

#include <vector>

namespace Chess
{
    struct Pos
    {
		Pos() {}
        Pos(int x, int y);
		bool operator == (const Pos& other) const;
		bool operator != (const Pos& other) const;
		Pos operator - (const Pos& other) const;
		bool IsValid() const;
		int LengthSqr() const;

		int x_ = 0;
		int y_ = 0;
    };

	using Positions = std::vector<Pos>;
}

