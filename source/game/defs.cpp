#include "defs.h"

Chess::Color Chess::GetOppositeColor(Color color)
{
    return color == Color::White ? Color::Black : Color::White;
}
