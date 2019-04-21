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

    using Pieces = std::vector<std::unique_ptr<Piece>>;

}
