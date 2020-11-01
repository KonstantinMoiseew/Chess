#pragma once

namespace Chess
{

    enum class Color
    {
        White,
        Black
    };

    enum class PieceType
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
