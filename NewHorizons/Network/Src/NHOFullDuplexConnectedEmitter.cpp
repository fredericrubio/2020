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

#include "NHOFullDuplexConnectedEmitter.hpp"

/**
 * Constructor
 **/
NHOFullDuplexConnectedEmitter::NHOFullDuplexConnectedEmitter(const unsigned short pDataPort,
                                                             const unsigned short pPeriod):
NHOEmitter(pDataPort, pPeriod) {
}

/**
 * Initiate whatever needs initialization.
 **/
bool NHOFullDuplexConnectedEmitter::initiate() {
    
    NHOFILE_LOG(logDEBUG) << "NHOFullDuplexConnectedEmitter::initiate." << std::endl;
    
    struct sockaddr_in lInfoServAddr;
    // service channel
    emissionSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (emissionSocket < 0) {
        fprintf(stderr, "ERROR opening socket");
        return(false);
    }
    bzero((char *) &lInfoServAddr, sizeof(lInfoServAddr));
    lInfoServAddr.sin_family = AF_INET;
    lInfoServAddr.sin_addr.s_addr = INADDR_ANY;
    lInfoServAddr.sin_port = htons(emissionPort);
    if (bind(emissionSocket, (struct sockaddr *) &lInfoServAddr, sizeof(lInfoServAddr)) < 0) {
        NHOFILE_LOG(logERROR) << "NHOFullDuplexConnectedEmitter::initiate: subscription socket opening error." << std::endl;
        return(false);
    }
    
    // launch the connexion thread
    connectionThread = new std::thread(&NHOFullDuplexConnectedEmitter::waitForConnectionOnSocket, std::ref(*this));

    NHOFILE_LOG(logDEBUG) << "NHOFullDuplexConnectedEmitter::initiate end." << std::endl;

    return(true);
    
}

////////////////////////////////
// Never ending loop.
// Wait and register a connexion on the main socket.
// This socket is the exit for captured images.
bool NHOFullDuplexConnectedEmitter::waitForConnectionOnSocket() {
    
    NHOFILE_LOG(logDEBUG) << "NHOFullDuplexConnectedEmitter::waitForConnectionOnDataSocket \n";

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
        ioctl(dataClientSocket, FIONBIO, &lMode);
        
        // an attempt to flush socket
        //        int flag = 1;
        //        setsockopt(dataClientSocket, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));

        NHOFILE_LOG(logDEBUG) << "NHOFullDuplexConnectedEmitter::waitForConnectionOnDataSocket another connection\n";
    }
    
    NHOFILE_LOG(logDEBUG) << "NHOFullDuplexConnectedEmitter::waitForConnectionOnDataSocket End\n";

    return(true);
    
}

///////////////////////////
// Send an image message.
bool NHOFullDuplexConnectedEmitter::send(const NHOMessage* pMsg) const {
    
    if (dataClientSocket <= 0) {
        NHOFILE_LOG(logERROR) << "NHOFullDuplexConnectedEmitter::send no data socket." << std::endl;
        return(false);
    }
    
    if (pMsg == NULL) {
        NHOFILE_LOG(logERROR) << "NHOFullDuplexConnectedEmitter::send parameter null." << std::endl;
        return(false);
    }
    
    // send message
    size_t lWrittenBytes = write(dataClientSocket, pMsg->getData(), pMsg->getSize());
    if (lWrittenBytes != pMsg->getSize()) {
        NHOFILE_LOG(logERROR) << "NHOFullDuplexConnectedEmitter IMP_Server::send " <<
        strerror(errno) << std::endl;

        NHOFILE_LOG(logERROR) << "NHOFullDuplexConnectedEmitter IMP_Server::send (number of written bytes:" <<
            lWrittenBytes << "/" << pMsg->getSize() << "))." << std::endl;
        return(false);
    }
    
    // wait for an answer
    // Useless: the sent image is in chunks (STREAM SOCKETS)
    NHOAckMessage* lAckMsg = new NHOAckMessage(clock());
    long lReceivedBytes;
    char* lBuffer = (char *) calloc(lAckMsg->getSize(), sizeof(char));
    lReceivedBytes = read(dataClientSocket, lBuffer, sizeof(size_t));
    // we can detect a problem in the transmission of the image
    if (lReceivedBytes < 0) {
        NHOFILE_LOG(logERROR) << "ERROR NHOFullDuplexConnectedEmitter::send (number of read bytes) " << lReceivedBytes << std::endl;
    }
    else {
        // check the answer
        lAckMsg->unserialize();
        if (lAckMsg->getValue() != pMsg->getSize()) {
            NHOFILE_LOG(logERROR) << "ERROR NHOFullDuplexConnectedEmitter::send: lost image (" << std::endl;
        }
    }
    // memory management
    delete lBuffer;
    delete lAckMsg;
    
    return(true);
}

bool NHOFullDuplexConnectedEmitter::echoing() {
    
    char buffer[256];
    long n;
    
    bzero(buffer,256);
    n = read(emissionSocket,buffer,255);
    if (n < 0) {
        std::cout << "ERROR reading from socket";
        return(false);
    }
    
    printf("Here is the message: %s\n",buffer);
    n = write(emissionSocket,"I got your message",18);
    if (n < 0) {
        std::cout << "ERROR writing to socket";
        return(false);
    }
    
    return(true);
    
}
