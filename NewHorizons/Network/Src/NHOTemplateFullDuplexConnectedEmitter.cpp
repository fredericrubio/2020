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

#include "NHOImage.hpp"
#include "NHOCameraDataMessage.hpp"
#include "NHOCameraData.hpp"

#include "NHOTemplateFullDuplexConnectedEmitter.hpp"
#ifdef TOTO
/**
 * Constructor
 **/
template <class T>
NHOTemplateFullDuplexConnectedEmitter<T>::NHOTemplateFullDuplexConnectedEmitter(
                                        const unsigned short pPort):
keepGoing(true) {
    
    emissionSocket = 0;
    connexionSocket = 0;

    message = new T();
    
}

/**
 * Initiate whatever needs initialization.
 **/
template <class T>
bool NHOTemplateFullDuplexConnectedEmitter<T>::initiate() {
    
    NHOFILE_LOG(logDEBUG) << "NHOTemplateFullDuplexConnectedEmitter::initiate." << std::endl;
    
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
        NHOFILE_LOG(logERROR) << "NHOTemplateFullDuplexConnectedEmitter::initiate " << strerror(errno) ;
        NHOFILE_LOG(logERROR) << "NHOTemplateFullDuplexConnectedEmitter::initiate: data socket binding error." << std::endl;
        return(false);
    }
     
    // launch the connexion thread
    connectionThread = new std::thread(&NHOTemplateFullDuplexConnectedEmitter::waitForConnectionOnSocket, std::ref(*this));

    NHOFILE_LOG(logDEBUG) << "NHOTemplateFullDuplexConnectedEmitter::initiate end." << std::endl;

    return(true);
     
}

////////////////////////////////
// Never ending loop.
// Wait and register a connexion on the main socket.
// This socket is the exit for captured images.
template <class T> bool NHOTemplateFullDuplexConnectedEmitter<T>::waitForConnectionOnSocket() {
    
    NHOFILE_LOG(logDEBUG) << "NHOTemplateFullDuplexConnectedEmitter::waitForConnectionOnDataSocket \n";

    socklen_t clilen;
    struct sockaddr_in cli_addr;
    
    listen(emissionSocket, 5);
    clilen = sizeof(cli_addr);
    
    while (keepGoing) {
        connexionSocket = accept( emissionSocket,
                                  (struct sockaddr *) &cli_addr,
                                  &clilen);
        if (connexionSocket < 0) {
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

        NHOFILE_LOG(logDEBUG) << "NHOTemplateFullDuplexConnectedEmitter::waitForConnectionOnDataSocket another connection\n";
    }
    
    NHOFILE_LOG(logDEBUG) << "NHOTemplateFullDuplexConnectedEmitter::waitForConnectionOnDataSocket End\n";

    return(true);
    
}

///////////////////////////
// Send a message.
template <class T> bool NHOTemplateFullDuplexConnectedEmitter<T>::send(const T* pMsg) const {
    
    if (connexionSocket <= 0) {
        NHOFILE_LOG(logERROR) << "NHOTemplateFullDuplexConnectedEmitter::send no data socket." << std::endl;
        return(false);
    }
    
    if (pMsg == NULL) {
        NHOFILE_LOG(logERROR) << "NHOTemplateFullDuplexConnectedEmitter::send parameter null." << std::endl;
        return(false);
    }
    
    // send message
    size_t lWrittenBytes = write(connexionSocket, pMsg->getData(), pMsg->getSize());
    if (lWrittenBytes != pMsg->getSize()) {
        NHOFILE_LOG(logERROR) << "NHOTemplateFullDuplexConnectedEmitter::send " << strerror(errno) << std::endl;
        NHOFILE_LOG(logERROR) << "NHOTemplateFullDuplexConnectedEmitter::send (number of written bytes:" <<
            lWrittenBytes << "/" << pMsg->getSize() << "))." << std::endl;
        return(false);
    }
    
    // wait for an answer
    // Useless: the sent image is in chunks (STREAM SOCKETS)
    NHOAckMessage* lAckMsg = new NHOAckMessage(clock());
    long lReceivedBytes;
    char* lBuffer = (char *) calloc(lAckMsg->getSize(), sizeof(char));
    lReceivedBytes = read(connexionSocket, lBuffer, sizeof(size_t));
    lAckMsg->setData(lBuffer);
    // we can detect a problem in the transmission of the image
    if (lReceivedBytes < 0) {
        NHOFILE_LOG(logERROR) << "ERROR NHOTemplateFullDuplexConnectedEmitter::send (number of read bytes) " << lReceivedBytes << std::endl;
    }
    else {
        // check the answer
        lAckMsg->unserialize();
        if (lAckMsg->getValue() != pMsg->getSize()) {
            NHOFILE_LOG(logERROR) << "ERROR NHOTemplateFullDuplexConnectedEmitter::send: lost image (" << std::endl;
        }
    }
    // memory management
    delete lAckMsg; // the variale lBuffer is freed when invoking destructor on lAckMsg
    
    return(true);
}
#endif
