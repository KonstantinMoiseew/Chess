#pragma once

#include "defs.h"
#include "position.h"
#include <vector>
#include <memory>

namespace Chess
{

    class Piece
    {
    public:

        enum class Type
        {
            Pawn,
            Knight,
            Bishop,
            Rook,
            Queen,
            King
        };

    protected:

        Color color;
        Type type;
        Pos position;

    };

    using PieceUnPtr = std::unique_ptr<Piece>;
    using Pieces = std::vector<PieceUnPtr>;

}
