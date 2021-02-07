#pragma once

#include <vector>
#include <optional>
#include <cassert>
#include <cmath>
#include "kernel/bytestream.h"
#include "kernel/math.h"

namespace Chess
{
    class Game;
    class IObserver;
    class Piece;
	struct Pos;
	class ICommand;
	class MoveCommand;
	class History;
}
