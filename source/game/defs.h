#pragma once

namespace Chess
{

    enum class Color : char
    {
        White = 1,
        Black = 2
    };

    enum class PieceType : char
	{
		Pawn,
		Knight,
		Bishop,
		Rook,
		Queen,
		King
	};

	const int BoardSize = 8;
}
