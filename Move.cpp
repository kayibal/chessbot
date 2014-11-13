//
//  Move.cpp
//  Chess
//
//  Created by Alan Höng on 11/11/14.
//  Copyright (c) 2014 Alan Höng. All rights reserved.
//

#include "Move.h"
using namespace BitBoard;
Move::Move(){
    piece = origin = target = capture = 0;
}
Move::Move(int p, u64 o, u64 t, int c){
    piece = p;
    origin = o;
    target = t;
    capture = c;
}
void Move::print(){
    int o_sq = (int)bitScanForward(origin);
    int t_sq = (int)bitScanForward(target);
    std:std::string type;
    if(piece == PAWN) type = "pawn";
    if(piece == KNIGHT) type = "knight";
    if(piece == ROOK) type = "rook";
    
    std::cout << "moving " << type << " from (" << getSquareX(o_sq) << ", " << getSquareY(o_sq) << ") to: (" << getSquareX(t_sq) << ", " << getSquareY(t_sq) << ")\n";
}