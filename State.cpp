//
//  State.cpp
//  Chess
//
//  Created by Alan Höng on 11/11/14.
//  Copyright (c) 2014 Alan Höng. All rights reserved.
//

#include "State.h"
using namespace BitBoard;

//pawn positions
const int pb_pos[64] = {
    1000,  1000,  1000,  1000,  1000,  1000,  1000,  1000,
    50, 50, 50, 50, 50, 50, 50, 50,
    10, 10, 20, 30, 30, 20, 10, 10,
    5,  5, 10, 25, 25, 10,  5,  5,
    0,  0,  0, 20, 20,  0,  0,  0,
    5, -5,-10,-20,-20,-10, -5,  5,
    5, 10, 10,-20,-20, 10, 10,  5,
    0,  0,  0,  0,  0,  0,  0,  0
};
const int pw_pos[64]={
     0,   0,   0,   0,   0,   0,   0,   0,
     5,  10,  10, -20, -20,  10,  10,   5,
     5,  -5, -10, -20, -20, -10,  -5,   5,
     0,   0,   0,  20,  20,   0,   0,   0,
     5,   5,  10,  25,  25,  10,   5,   5,
    10,  10,  20,  30,  30,  20,  10,  10,
    50,  50,  50,  50,  50,  50,  50,  50,
     1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000
};


//knight positions (symmetric)
const int k_pos[64] = {
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50,
};

//rook positions
const int rb_pos[64] = {
    0,  0,  0,  0,  0,  0,  0,  0,
    5, 10, 10, 10, 10, 10, 10,  5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    0,  0,  0,  5,  5,  0,  0,  0
};
const int rw_pos[64] = {
     0,  0,  0,  5,  5,  0,  0,  0,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
     5, 10, 10, 10, 10, 10, 10,  5,
     0,  0,  0,  0,  0,  0,  0,  0
};

State::State(bool c){
    color = c;
    white = black = pawns = knights = rooks = 0;
    value = depth = 0;
}
State::~State(){
    delete(prev);
    for (int i = 0; i < children.size(); i++){
        delete(children[i]);
    }
    
}
void State::addChild(State* s){
    children.push_back(s);
}
int State::evaluateState(){
    int score = 0;
    u64 own_pieces;
    color ? own_pieces = white : own_pieces =black;
    
    u64 pawns = own_pieces & this->pawns;
    u64 knights = own_pieces & this->knights;
    u64 rooks = own_pieces & this->rooks;
    
    int k = (int)popcount(knights);
    int p = (int)popcount(pawns);
    int r = (int)popcount(rooks);
    
    int material = p*100 + k*300*(1+p/16) + r*1000*(1-(p/6));
    int position = 0;
    
    int sqno = 0;
    while(pawns){
        sqno = (int)bitScanForward(pawns);
        if(color){
            position += pw_pos[sqno];
        } else {
            position += pb_pos[sqno];
        }
        pawns = pawns & (pawns-1);
    }
    while(knights){
        sqno = (int)bitScanForward(knights);
        if(color){
            position += pw_pos[sqno];
        } else {
            position += pb_pos[sqno];
        }
        knights = knights & (knights-1);
    }
    while(rooks){
        sqno = (int)bitScanForward(rooks);
        if(color){
            position += pw_pos[sqno];
        } else {
            position += pb_pos[sqno];
        }
        rooks = rooks & (rooks-1);
    }
    score = material + position;
    if(color){
        return score;
    } else {
        return -score;
    }
}
void State::loadState(std::string board){
    int sqno;
    int i = 0;
    for (int j = 0; j < 8; j++){
        sqno = 56-(j*8);
        for (int k = 0; k < 8; k++){
            if(board[i]!='.'){
                if(board[i] == 'P'){
                    white |= BitSet[sqno];
                    pawns |= BitSet[sqno];
                }else if(board[i] == 'p'){
                    black |= BitSet[sqno];
                    pawns |= BitSet[sqno];
                }else if(board[i] == 'N'){
                    white |= BitSet[sqno];
                    knights |= BitSet[sqno];
                }else if(board[i] == 'n'){
                    black |= BitSet[sqno];
                    knights |= BitSet[sqno];
                }else if(board[i] == 'R'){
                    white |= BitSet[sqno];
                    rooks |= BitSet[sqno];
                }else if(board[i] == 'r'){
                    black |= BitSet[sqno];
                    rooks |= BitSet[sqno];
                }
            }
            sqno++;
            i++;
        }
    }
}

Move* State::getBestMove(){
    Move* best = nullptr;
    int max_val = -99999999;
    for (int i = 0; i < children.size(); i++){
        if (children[i]->value > max_val){
            max_val = children[i]->value;
            best = children[i]->prev;
        }
    }
    return best;
}

void State::print(){
    std::string board = std::string(64,' ');
    u64 rooks = this->rooks;
    u64 pawns = this->pawns;
    u64 knights = this->knights;
    while(rooks){
        int sqno = (int) bitScanForward(rooks);
        if (black & BitSet[sqno]){
            board[sqno] = 'r';
        } else {
            board[sqno] = 'R';
        }
        rooks = rooks & (rooks-1);
    }
    while(pawns){
        int sqno = (int) bitScanForward(pawns);
        if (black & BitSet[sqno]){
            board[sqno] = 'p';
        } else {
            board[sqno] = 'P';
        }
        pawns = pawns & (pawns-1);
    }
    while(knights){
        int sqno = (int) bitScanForward(knights);
        if (black & BitSet[sqno]){
            board[sqno] = 'n';
        } else {
            board[sqno] = 'N';
        }
        knights = knights & (knights-1);
    }
    //reverse string
    board = std::string(board.rbegin(),board.rend());
    std::string squares;
    int r;
    std::cout << "\n  --------------------------------\n";
    for(int i = 0; i < 8; i++){
        r = 7-i;
        squares = board.substr(i*8,8);
        std::cout << r <<"| ";
        for (int j = 0; j < 8; j++){
            std::cout << squares[j] << " | ";
        }
        std::cout << "\n  --------------------------------\n";
    }
    std::cout << " | A | B | C | D | E | F | G | H |\n";
    
}