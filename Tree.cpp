//
//  Tree.cpp
//  Chess
//
//  Created by Alan Höng on 11/11/14.
//  Copyright (c) 2014 Alan Höng. All rights reserved.
//

#include "Tree.h"
#include <cassert>
#include <climits>

using namespace BitBoard;
Tree::Tree(bool c, int levels):wpa(64),bpa(64),bpm(64),wpm(64),km(64){
    color = c;
    max_level = levels;
    generateAllMoves();
    root = nullptr;
}

/*
 
 Generates all possible moves for a given state
 
 */
std::vector<Move*> Tree::generateAllMoves(State& s){
    bool color = s.color;
    u64 own_pieces, opp_pieces, all_pieces, row, invalid_start_bit, pos;
    std::vector<u64>& pawn_movements = bpm;
    std::vector<u64>& pawn_attack = bpa;
    
    if(color){
        own_pieces = s.white;
        opp_pieces = s.black;
        row = ((u64)255)<<8;
        pawn_movements = wpm;
        pawn_attack = wpa;
    } else {
        own_pieces = s.black;
        opp_pieces = s.white;
        row = ((u64)255)<<48;
    }
    
    all_pieces = opp_pieces | own_pieces;
    
    //my own pieces for iteration
    u64 my_pawns = own_pieces & s.pawns;
    u64 my_knights = own_pieces & s.knights;
    u64 my_rooks = own_pieces & s.rooks;
    
    //stores attacks on certain pieces
    u64 c_knights, c_rooks, c_pawns;
    
    std::vector<Move*> moves;
    int sqno;
    u64 move_mask;
    u64 attack_mask;
    std::vector<Move*> temp;
    
    while(my_pawns){
        sqno = (int)bitScanForward(my_pawns);
        move_mask = pawn_movements[sqno];
        attack_mask = pawn_attack[sqno];
        pos = BitSet[sqno];
        
        /*
         ==========================
        treating possible attacks
         ==========================
         */
        c_knights = attack_mask & (s.knights & opp_pieces);
        c_rooks = attack_mask & (s.rooks & opp_pieces);
        c_pawns = attack_mask & (s.pawns & opp_pieces);
        
        //add possible captures to moves vector
        temp = createMoves(c_rooks, pos, PAWN, ROOK);
        moves.insert(moves.end(),temp.begin(),temp.end());
        temp = createMoves(c_knights, pos, PAWN, KNIGHT);
        moves.insert(moves.end(),temp.begin(),temp.end());
        temp = createMoves(c_pawns, pos, PAWN, PAWN);
        moves.insert(moves.end(),temp.begin(),temp.end());
        
        /*
         ==========================
         treating possible moves
         ==========================
         */
        //pawns can only move if no other piece is in the way
        move_mask &= ~all_pieces;
        //printBitBoard(move_mask);
        if (row & pos){
            //special case pawn is in startpos but first square is blocked
            if (color)
                invalid_start_bit = pos << 8;
            else
                invalid_start_bit = pos >> 8;
            if(!(invalid_start_bit & all_pieces)){
                //no one in the way move!
                temp = createMoves(move_mask, pos, PAWN,0);
                moves.insert(moves.end(),temp.begin(),temp.end());
            } else {
                temp = createMoves(move_mask, pos, PAWN,0);
                moves.insert(moves.end(),temp.begin(),temp.end());
            }
                
        }
        
        my_pawns &= my_pawns-1;
    }
    
    while (my_knights){
        sqno = bitScanForward(my_knights);
        pos = BitSet[sqno];
        move_mask = km[sqno];
        
        move_mask &= ~own_pieces;
        c_knights = move_mask & (s.knights & opp_pieces);
        c_rooks = move_mask & (s.rooks & opp_pieces);
        c_pawns = move_mask & (s.pawns & opp_pieces);
        
        //remove captures from move mask
        move_mask &= ~c_knights;
        move_mask &= ~c_rooks;
        move_mask &= ~c_pawns;
        
        temp = createMoves(c_rooks, pos, KNIGHT, ROOK);
        moves.insert(moves.end(),temp.begin(),temp.end());
        temp = createMoves(c_knights, pos, KNIGHT, KNIGHT);
        moves.insert(moves.end(),temp.begin(),temp.end());
        temp = createMoves(c_pawns, pos, KNIGHT, PAWN);
        moves.insert(moves.end(),temp.begin(),temp.end());
        temp = createMoves(move_mask, pos, KNIGHT, 0);
        moves.insert(moves.end(),temp.begin(),temp.end());
        
        my_knights &= my_knights-1;
    }
    while(my_rooks){
        sqno = bitScanForward(my_rooks);
        pos = BitSet[sqno];
        move_mask = getRookMoveMask(all_pieces, sqno);
        
        move_mask &= ~own_pieces;
        c_rooks = move_mask & (s.rooks & opp_pieces);
        c_knights = move_mask & (s.knights & opp_pieces);
        c_pawns = move_mask & (s.pawns & opp_pieces);
        
        //remove captures from move mask
        move_mask &= ~c_knights;
        move_mask &= ~c_rooks;
        move_mask &= ~c_pawns;
        
        temp = createMoves(c_rooks,pos,ROOK,ROOK);
        moves.insert(moves.end(),temp.begin(),temp.end());
        temp = createMoves(c_knights,pos,ROOK,KNIGHT);
        moves.insert(moves.end(),temp.begin(),temp.end());
        temp = createMoves(c_pawns,pos,ROOK,PAWN);
        moves.insert(moves.end(),temp.begin(),temp.end());
        temp = createMoves(move_mask,pos,ROOK,0);
        moves.insert(moves.end(),temp.begin(),temp.end());
        
        my_rooks&= my_rooks-1;
    }
    return moves;
}
int max2(int a, int b){
    if (a >= b){
        return a;
    }
    return b;
}
int min2(int a, int b){
    if (a <= b){
        return a;
    }
    return b;
}
int Tree::alphaBeta(State *node, int depth, int alpha, int beta, bool max){
    State* n;
    if (depth <= 0)
        return node->evaluateState();
    
    std::vector<Move*> moves = generateAllMoves(*node);
    //sort moves here
    if(max){
        for(int i = 0; i < moves.size(); i++){
            n = makeMove(node, moves[i]);
            alpha = max2(alpha, alphaBeta(n, depth-1, alpha, beta, false));
            node->value = alpha;
            if (depth == max_level)
                node->addChild(n);
            else
                delete(n);
            if(beta <= alpha){
                break;
            }
        }
        return alpha;
    } else {
        for(int i = 0; i < moves.size(); i++){
            n = makeMove(node, moves[i]);
            beta = min2(beta, alphaBeta(n, depth-1, alpha, beta, true));
            node->value = beta;
            if (depth == max_level)
                node->addChild(n);
            else
                delete(n);
            if(beta <= alpha){
                break;
            }
        }
        return beta;
    }
}

State* Tree::makeMove(State* current, Move* move){
    State* n = new State(!current->color);
    u64 rooks, pawns, knights, own_pieces, opponent_pieces, target, origin;

    //this new state is reached by this move
    n->prev = move;
    
    current->color ? own_pieces = current->white : own_pieces = current->black;
    current->color ? opponent_pieces = current->black : opponent_pieces = current->white;
    rooks = current->rooks;
    pawns = current->pawns;
    knights = current->knights;
    target = move->target;
    origin = move->origin;
    assert(target && ((target & own_pieces) == 0));
    
    //make move in own pieces as only we are moving
    own_pieces = (own_pieces | target) & ~origin;
    
    if(move->piece == PAWN){
        //add target to pawns
        pawns |= target;
        //remove origin from pawns
        pawns &= ~origin;
    }else if(move->piece == KNIGHT){
        //add target to knights
        knights |= target;
        //remove origin from knights
        knights &= ~origin;
    }else if(move->piece == ROOK){
        //add target to rooks
        rooks |= target;
        //remove origin from rooks
        rooks &= ~origin;
    }
    if(move->capture > 0){
        opponent_pieces &= ~target;
        if(move->capture == PAWN){
            //remove one pawn
            pawns &= ~target;
        } else if(move->capture == KNIGHT){
            knights &= ~target;
        } else if(move->capture == ROOK){
            rooks &= ~target;
        }
    }
    n->pawns = pawns;
    n->knights = knights;
    n->rooks = rooks;
    
    if(current->color){
        n->white = own_pieces;
        n->black = opponent_pieces;
    } else {
        n->white = opponent_pieces;
        n->black = own_pieces;
    }
    
    return n;
}
//Creates all moves for one piece
std::vector<Move*> Tree::createMoves(u64 move_mask, u64 current_pos, int piece, int capture){
    std::vector<Move*> moves;
    while(move_mask){
        u64 target = BitSet[bitScanForward(move_mask)];
        Move* m = new Move(piece, current_pos, target, capture);
        moves.push_back(m);
        move_mask &= (move_mask-1);
    }
    return moves;
}
#warning pawns don't need full board spare out last and first line
void Tree::generateAllMoves(){
    int no;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            no = getSquareNo(i,j);
            if(j >= 1 && j <=6){
                wpa[no] = createPawnAttackMask(i, j, true);
                bpa[no] = createPawnAttackMask(i, j, false);
                bpm[no] = createPawnMoveMask(i, j, false);
                wpm[no] = createPawnMoveMask(i, j, true);
                std::cout << i << ", " << j << "\n";
                printBitBoard(bpa[no]);
            }
            km[no] = getKnightMoveMask(i, j);
        }
    }
}

Move Tree::nextMove(std::string board){
    if (root != nullptr){
        delete root;
    }
    root = new State(color);
    root->loadState(board);
    alphaBeta(root, max_level, INT_MIN, INT_MAX, color);
    Move next = *(root->getBestMove());
    return next;
}