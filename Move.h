//
//  Move.h
//  Chess
//
//  Created by Alan Höng on 11/11/14.
//  Copyright (c) 2014 Alan Höng. All rights reserved.
//

#ifndef __Chess__Move__
#define __Chess__Move__

#include <iostream>
#include "BitBoard.h"

const int PAWN = 1;
const int KNIGHT = 2;
const int ROOK = 4;

class Move{
public:
    int piece;
    u64 origin;
    u64 target;
    int capture;
    
    Move();
    Move(int piece, u64 origin, u64 target, int capture);
    void print();
};
#endif /* defined(__Chess__Move__) */
