#pragma once

namespace Chess
{
    struct Pos
    {
        int x_ ;
        int y_ ;
        Pos();
        Pos(int x, int y);


		bool operator == (const Pos& other) const
		{
			return x_ == other.x_ && y_ == other.y_;
		}
    };
}

