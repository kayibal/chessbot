//
//  main.cpp
//  Chess
//
//  Created by Alan Höng on 11/11/14.
//  Copyright (c) 2014 Alan Höng. All rights reserved.
//

#include <iostream>
#include "Tree.h"
#include <boost/asio.hpp>
#include <boost/array.hpp>

using boost::asio::ip::tcp;

int main(int argc, const char * argv[])
{
    bool color = false;
    int levels = 5;
    std::string botname = "Alan";
    std::string host = "127.0.0.1";
    unsigned short port = 50100;
    
    //use name color levels
    if (argc < 4){
        std::cout << "usage: bot <name> <host> <port> <color> <levels> \n";
    } else {
        botname = std::string(argv[1],NULL,0);
        color = (bool) strtol(argv[2], NULL, 0);
        levels = (int) strtol(argv[3], NULL, 0);
    }
    
    //init game tree
    Tree game(color,levels);
    
    int winner = 0;
    std::string board = "rn....nr"
                        "pppppppp"
                        "........"
                        "........"
                        "........"
                        "........"
                        "PPPPPPPP"
                        "RN....NR";
    
    std::string message = "{\"name\":\"" + botname + "\"}";
    std::cout << message;
    
    //setup some basic boost variables
    boost::asio::io_service io_service;
    tcp::socket socket(io_service);
    boost::asio::ip::address ip = boost::asio::ip::address::from_string(host);
    tcp::endpoint endpoint(ip,port);
    //socket.connect(endpoint);
    
    boost::array<char, 256> buf;
    boost::system::error_code error;
    
    //send name
    /*
    boost::system::error_code ignored_error;
    boost::asio::write(socket, boost::asio::buffer(message),
                       boost::asio::transfer_all(), ignored_error);
    */
    //while (winner == 0){
    
        //get status
        /*
        size_t len = socket.read_some(boost::asio::buffer(buf), error);
        std::cout.write(buf.data(), len);
         */
    
        Move m = game.nextMove(board);
        m.print();
        BitBoard::printBitBoard(m.origin | m.target);
        return 0;
    //}
    
    

}

