//
//  File.cpp
//  Chess
//
//  Created by Alan Höng on 11/11/14.
//  Copyright (c) 2014 Alan Höng. All rights reserved.
//

#include "BitBoard.h"
using namespace BitBoard;

//make this constant somehow
std::vector<u64> BitBoard::BitSet = createBitSet();

//internal use
u64 k1 = 0x5555555555555555;
u64 k2 = 0x3333333333333333;
u64 k4 = 0x0f0f0f0f0f0f0f0f;
u64 kf = 0x0101010101010101;

u64 BitBoard::bitReversal(u64 x){
    x = (((x & 0xaaaaaaaaaaaaaaaa) >> 1) | ((x & 0x5555555555555555) << 1));
    x = (((x & 0xcccccccccccccccc) >> 2) | ((x & 0x3333333333333333) << 2));
    x = (((x & 0xf0f0f0f0f0f0f0f0) >> 4) | ((x & 0x0f0f0f0f0f0f0f0f) << 4));
    x = (((x & 0xff00ff00ff00ff00) >> 8) | ((x & 0x00ff00ff00ff00ff) << 8));
    x = (((x & 0xffff0000ffff0000) >> 16) | ((x & 0x0000ffff0000ffff) << 16));
    return ((x >> 32) | (x << 32));
}

std::vector<u64> BitBoard::createBitSet(){
    std::vector<u64> B = std::vector<u64>(64);
    for (int i = 0; i < 64; i++){
        B[i] = ((u64)1) << i;
    }
    return B;
}

 u64 BitBoard::popcount(u64 x){
    return __builtin_popcountll(x);
}

u64 BitBoard::bitScanForward(u64 x){
    return __builtin_ctzll(x);
}

int BitBoard::getSquareNo(int i, int j){
    if( j >= 0 && j <= 7 &&  i>=0 && i <=7){
        return j*8+i;
    }
    //std::cout << "getSquareNo out of bounds \n";
    return -1;
}
int BitBoard::getSquareX(int no){
    if(no > 63 || no < 0){
        std::cout << "error sqno not on board\n";
        return -1;
    }
    return no%8;
}
int BitBoard::getSquareY(int no){
    if(no > 63 || no < 0){
        std::cout << "error sqno not on board\n";
        return -1;
    }
    return no/8;
}
u64 BitBoard::createPawnMoveMask(int i, int j, bool color){
    u64 mask = 0;
    if(j>=1 && j<= 6){
        if(color){
            //white
            mask = BitSet[getSquareNo(i, j+1)];
            if(j==1){
                mask |= BitSet[getSquareNo(i,j+2)];
            }
        } else {
            //white
            mask = BitSet[getSquareNo(i, j-1)];
            if(j==6){
                mask |= BitSet[getSquareNo(i,j-2)];
            }
        }
    } else {
        std::cout<<"createPawnMoveMask Error pawn moves into last or first row\n";
    }
    return mask;
}

u64 BitBoard::createPawnAttackMask(int i, int j, bool color){
    u64 mask = 0;
    if( j >= 1 && j <= 6 &&  i >= 1 && i <= 6){
        if(color){
            mask = BitSet[getSquareNo(i-1,j+1)] | BitSet[getSquareNo(i+1,j+1)];
        } else {
            mask = BitSet[getSquareNo(i-1,j-1)] | BitSet[getSquareNo(i+1,j-1)];
        }
    }
    if (i==0){
        if(color){
            mask = BitSet[getSquareNo(i+1,j+1)];
        } else {
            mask = BitSet[getSquareNo(i+1,j-1)];
        }
    }
    if (i == 7){
        if(color){
            mask = BitSet[getSquareNo(i-1,j+1)];
        } else {
            mask = BitSet[getSquareNo(i-1,j-1)];
        }
    }
    return mask;
}

u64 BitBoard::getRookMoveMask(u64 all_pieces, int sqno){
    int i = getSquareX(sqno);
    int j = getSquareY(sqno);
    u64 row_mask = ((u64)255)<<j*8;
    u64 file_mask = getFileMask(i);
    u64 current_pos = BitSet[sqno];
    u64 occ = all_pieces & row_mask;
    u64 occ_r = bitReversal(occ);
    u64 current_pos_r = bitReversal(current_pos);
    
    //Hyperbola Quintessence Method
    //https://chessprogramming.wikispaces.com/Hyperbola+Quintessence
    
    u64 line_moves = row_mask & ((occ - 2*current_pos) ^ bitReversal(occ_r-2*current_pos_r));
    
    occ = all_pieces & file_mask;
    occ_r = bitReversal(occ);
    u64 file_moves = file_mask & ((occ - 2*current_pos) ^ bitReversal(occ_r-2*current_pos_r));
    
    return line_moves | file_moves;
    
}

u64 BitBoard::getKnightMoveMask(int i, int j){
    u64 mask = 0;
    //all possible 8 position in touples might be outside of board
    int pos[16] = {i+1,j+2,i+2,j+1,i+1,j-2,i+2,j-1,i-1,j+2,i-2,j+1,i-1,j-2,i-2,j-1};
    int x,y,no;
    for (int i = 0; i < 16; i+=2){
        x = pos[i];
        y = pos[i+1];
        no = getSquareNo(x, y);
        //if inside board add to mask
        if (no >= 0){
            mask |= BitSet[no];
        }
    }
    return mask;
}

u64 BitBoard::getFileMask(int i){
    u64 mask = 0;
    if (i >= 0 && i <= 7){
        mask = BitSet[i] | BitSet[i+8] | BitSet[i+16] | BitSet[i+24] | BitSet[i+32] | BitSet[i+40] | BitSet[i+48] | BitSet[i+56];
        
    }
    return mask;
}

u64 BitBoard::mirrorH(u64 x){
	x = ((x >> 1) & k1) | ((x & k1) << 1);
	x = ((x >> 2) & k2) | ((x & k2) << 2);
	x = ((x >> 4) & k4) | ((x & k4) << 4);
	return x;
}

u64 BitBoard::mirrorV(u64 x){
    return ( (x << 56) ) | ( (x << 40) & 0x00ff000000000000 ) | ( (x << 24) & 0x0000ff0000000000 ) |( (x <<  8) & 0x000000ff00000000 ) |( (x >>  8) & 0x00000000ff000000 ) |( (x >> 24) & 0x0000000000ff0000 ) | ( (x >> 40) & 0x000000000000ff00 ) |( (x >> 56) );
}

void BitBoard::printBitBoard(u64 b){
    std::bitset<64> x(b);
    std::string bitboard = x.to_string();
    std::string squares;
    for(int i = 0; i < 8; i++){
        int r = 7-i;
        squares = bitboard.substr(i*8,8);
        squares = std::string(squares.rbegin(),squares.rend());
        std::cout << r << " |";
        for (int j = 0; j < squares.length(); j++){
            std::cout << squares[j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "   ---------------\n";
    std::cout << "   0 1 2 3 4 5 6 7\n";
}
