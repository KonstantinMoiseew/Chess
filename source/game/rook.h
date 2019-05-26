#pragma once
#include "piece.h"
using namespace Chess;

class Rook: public Piece
{
public:
		Rook(Type type, Color color, Pos position);
		//void SetPos(double x, double y)
		//{
			//	x=0;
			//	y=0;
	//	}

};

Rook:: Rook(Type type, Color color, Pos position):Piece( type, color, position)
{

}
