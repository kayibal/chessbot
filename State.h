//
//  State.h
//  Chess
//
//  Created by Alan Höng on 11/11/14.
//  Copyright (c) 2014 Alan Höng. All rights reserved.
//

#ifndef __Chess__State__
#define __Chess__State__

#include <iostream>
#include <vector>
#include <cstdint>
#include "BitBoard.h"
#include "Move.h"

class State{
public:
    u64 white;
    u64 black;
    u64 pawns;
    u64 knights;
    u64 rooks;
    
    Move* prev;
    
    int value;
    int depth;
    bool color;
    std::vector<State*> children;
    
    //methods
    State(bool c);
    ~State();
    void loadState(std::string board);
    void addChild(State* c);
    int evaluateState();
    Move* getBestMove();
    void print();
};
#endif /* defined(__Chess__State__) */
