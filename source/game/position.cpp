#include "position.h"

Chess::Pos::Pos(int x, int y)
{
    x_=x;
    y_=y;
}

bool Chess::Pos::operator == (const Pos& other) const
{
	return x_ == other.x_ && y_ == other.y_;
}

Chess::Pos Chess::Pos::operator - (const Pos& other) const
{
	return Pos(x_ - other.x_, y_ - other.y_);
}

bool Chess::Pos::IsValid() const
{
	return x_ >= 0 && x_ < 8 && y_ >= 0 && y_ < 8;
}

int Chess::Pos::LengthSqr() const
{
	return x_*x_ + y_*y_;
}
