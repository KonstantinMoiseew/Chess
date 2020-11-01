#pragma once

#include <QString>
#include "game/defs.h"

inline QString GetPiecePixmapPath(Chess::PieceType type, Chess::Color color)
{
		QString image_name = ":/graphics/pieces/";
		switch (type)
		{
        case Chess::PieceType::Pawn: image_name += "pawn"; break;
        case Chess::PieceType::Knight: image_name += "knight"; break;
        case Chess::PieceType::Bishop: image_name += "bishop"; break;
        case Chess::PieceType::Rook : image_name += "rook"; break;
        case Chess::PieceType::Queen: image_name += "queen"; break;
        case Chess::PieceType::King: image_name += "king"; break;
		}

		if (color == Chess::Color::White)
			image_name += "_white";
		else
			image_name += "_black";

		image_name += ".png";

		return image_name;
}
