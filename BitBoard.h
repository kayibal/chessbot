//
//  File.h
//  Chess
//
//  Created by Alan Höng on 11/11/14.
//  Copyright (c) 2014 Alan Höng. All rights reserved.
//

#ifndef __Chess__BitBoard__
#define __Chess__BitBoard__

#include <iostream>
#include <cstdint>
#include <vector>
typedef uint64_t u64;
namespace BitBoard{
    extern std::vector<u64> BitSet;
    
    std::vector<u64> createBitSet();
    u64 bitReversal(u64 x);
    u64 popcount(u64 x);
    u64 bitScanForward(u64 x);
    int getSquareX(int no);
    int getSquareY(int no);
    int getSquareNo(int i, int j);
    u64 bitScanForward(u64 x);
    u64 getFileMask(int i);
    u64 mirrorH(u64 x);
    u64 mirrorV(u64 x);
    u64 createPawnMoveMask(int i, int j, bool color);
    u64 createPawnAttackMask(int i, int j, bool color);
    u64 getRookMoveMask(u64 all_pieces, int sqno);
    u64 getKnightMoveMask(int i, int j);
    void printBitBoard(u64 b);
}

#endif /* defined(__Chess__File__) */
