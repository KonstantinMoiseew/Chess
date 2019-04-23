#pragma once

#include <vector>
#include <memory>

namespace Chess
{

    class Piece
    {
    public:

    protected:


    };

    using PieceUnPtr = std::unique_ptr<Piece>;
    using Pieces = std::vector<PieceUnPtr>;

}
