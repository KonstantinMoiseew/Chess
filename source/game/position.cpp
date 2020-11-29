#include "position.h"
#include "defs.h"
#include "stdio.h"

Chess::Pos::Pos(int x, int y)
{
    x_=x;
    y_=y;
}

bool Chess::Pos::operator == (const Pos& other) const
{
	return x_ == other.x_ && y_ == other.y_;
}

bool Chess::Pos::operator != (const Pos& other) const
{
	return !(*this==other);
}

Chess::Pos Chess::Pos::operator - (const Pos& other) const
{
	return Pos(x_ - other.x_, y_ - other.y_);
}

Chess::Pos Chess::Pos::operator + (const Pos& other) const
{
	return Pos(x_ + other.x_, y_ + other.y_);
}

bool Chess::Pos::IsValid() const
{
    return x_ >= 0 && x_ < BoardSize && y_ >= 0 && y_ < BoardSize;
}

int Chess::Pos::LengthSqr() const
{
	return x_*x_ + y_*y_;
}

std::string Chess::Pos::ToString() const
{
	char buf[16];
	sprintf(buf, "%c%i", 'a' + x_, y_+1);

	return buf;
}

