#pragma once

#include <vector>
#include <optional>
#include <cassert>
#include "kernel/bytestream.h"

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
