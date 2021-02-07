#pragma once

namespace Chess
{

    enum class Color : char
    {
        White = 1,
        Black = 2
    };


    Color GetOppositeColor(Color color);

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
