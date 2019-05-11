//
//  NHOFullDuplexConnectedEmitter.cpp
//  Network
//
//  Created by Frédéric Rubio on 27/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <mutex>
#include <sys/ioctl.h>

#include "NHOLOG.hpp"
#include "NHOMessage.hpp"
#include "NHOAckMessage.hpp"
#include "NHOAckMessage.hpp"

#include "NHOReflexFullDuplexConnectedEmitter.hpp"

/**
 * Constructor
 **/
NHOReflexFullDuplexConnectedEmitter::NHOReflexFullDuplexConnectedEmitter(const unsigned short pDataPort):
NHOFullDuplexConnectedEmitter(pDataPort, 0) {

    emissionSocket = 0;
    dataClientSocket = 0;
    message = NULL;
    
}

////////////////////////////////
// Never ending loop.
// Wait and register a connexion on the main socket.
// This socket is the exit for captured images.
bool NHOReflexFullDuplexConnectedEmitter::waitForConnectionOnSocket() {
    
    NHOFILE_LOG(logDEBUG) << "NHOReflexFullDuplexConnectedEmitter::waitForConnectionOnDataSocket \n";

    socklen_t clilen;
    struct sockaddr_in cli_addr;
    
    listen(emissionSocket, 5);
    clilen = sizeof(cli_addr);
    
    while (1) {
        dataClientSocket = accept(emissionSocket,
                                  (struct sockaddr *) &cli_addr,
                                  &clilen);
        if (dataClientSocket < 0) {
            fprintf(stderr, "ERROR on accept");
            return(false);
        }
        // Set the socket I/O mode: In this case FIONBIO
        // enables or disables the blocking mode for the
        // socket based on the numerical value of iMode.
        // If iMode = 0, blocking is enabled;
        // If iMode != 0, non-blocking mode is enabled.
        int lMode = 0;
        ioctl(emissionSocket, FIONBIO, &lMode);
        
        // an attempt to flush socket
        //        int flag = 1;
        //        setsockopt(dataClientSocket, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));
        // send all service messages
//        new std::thread(&NHOReflexFullDuplexConnectedEmitter::send, std::ref(*this), message);
        send(message);
        
        NHOFILE_LOG(logDEBUG) << "NHOReflexFullDuplexConnectedEmitter::waitForConnectionOnDataSocket another connection\n";
    }
    
    NHOFILE_LOG(logDEBUG) << "NHOReflexFullDuplexConnectedEmitter::waitForConnectionOnDataSocket End\n";

    return(true);
    
}
