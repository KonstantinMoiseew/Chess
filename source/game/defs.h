#pragma once

namespace Chess
{

    enum class Color
    {
        White,
        Black
    };

	enum class Type
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
