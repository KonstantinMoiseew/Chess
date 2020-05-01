#pragma once

#include <QString>
#include "game/defs.h"

inline QString GetPiecePixmapPath(Chess::Type type, Chess::Color color)
{
		QString image_name = ":/graphics/pieces/";
		switch (type)
		{
		case Chess::Type::Pawn: image_name += "pawn"; break;
		case Chess::Type::Knight: image_name += "knight"; break;
		case Chess::Type::Bishop: image_name += "bishop"; break;
		case Chess::Type::Rook : image_name += "rook"; break;
		case Chess::Type::Queen: image_name += "queen"; break;
		case Chess::Type::King: image_name += "king"; break;
		}

		if (color == Chess::Color::White)
			image_name += "_white";
		else
			image_name += "_black";

		image_name += ".png";

		return image_name;
}
