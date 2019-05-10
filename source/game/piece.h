#pragma once

#include "defs.h"
#include "position.h"
#include <vector>
#include <memory>
#include "type.h"

namespace Chess
{

    class Piece
    {
    public:


          Piece(Type type, Color color, Pos position)
          {
              color_=color;
              type_=type;
              position_=position;

          }

        Color get_color()
        {
            return color_;
        }
        Type get_type()
        {
            return type_;
        }
        Pos get_pos()
        {
            return position_;
        }

    protected:

        Color color_ = Color::White;
        Type type_ = Type::Pawn;
        Pos position_;

    };

    using PieceUnPtr = std::unique_ptr<Piece>;
    using Pieces = std::vector<PieceUnPtr>;

}
