//
//  Tree.h
//  Chess
//
//  Created by Alan Höng on 11/11/14.
//  Copyright (c) 2014 Alan Höng. All rights reserved.
//

#ifndef __Chess__Tree__
#define __Chess__Tree__

#include <iostream>
#include <vector>
#include "State.h"
#include "Move.h"
#include "BitBoard.h"

class Tree{
    State* root;
    bool color;
    int max_level;
    
    //precalcalculated movement masks
    std::vector<u64> wpa;
    std::vector<u64> bpa;
    std::vector<u64> bpm;
    std::vector<u64> wpm;
    std::vector<u64> km;
    
    std::vector<Move*> generateAllMoves(State& s);
    int alphaBeta(State* node, int depth, int alpha, int beta, bool max);
    State* makeMove(State* current, Move* move);
    std::vector<Move*> createMoves(u64 move_mask, u64 current_pos, int piece, int capture);
    void generateAllMoves();
    
public:
    Tree(bool c, int levels);
    Move nextMove(std::string board);
    
};

#endif /* defined(__Chess__Tree__) */
