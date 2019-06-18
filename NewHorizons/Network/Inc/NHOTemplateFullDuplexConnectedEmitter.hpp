//
//  NHOFullDuplexConnectedEmitter.hpp
//  Network
//
//  Created by Frédéric Rubio on 27/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOFullDuplexConnectedEmitter_hpp
#define NHOFullDuplexConnectedEmitter_hpp

#include <stdio.h>
#include <thread>
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
#include <list>
#include <mutex>
#include "NHOLOG.hpp"
#include "NHOAckMessage.hpp"

class NHOMessage;

template <class T> class NHOTemplateFullDuplexConnectedEmitter {
    
public:
    /**
     * Constructor
     **/
    NHOTemplateFullDuplexConnectedEmitter(const unsigned short pPort):
    keepGoing(true) {
        
        emissionSocket = 0;
        connexionSocketList.clear();
        emissionPort = pPort;
        
    }

    /**
     * To terminate safely.
     **/
    bool terminate() {
        
        keepGoing = false;
        for (int connexionSocket : connexionSocketList) {
            close(connexionSocket);
        }
        close(emissionSocket);
        return true;
        
    }
    
    /**
     * Initialize whatever lust be initialized.
     **/
    virtual bool initiate() {
        
        NHOFILE_LOG(logDEBUG) << "NHOTemplateFullDuplexConnectedEmitter::initiate." << std::endl;
        
        struct sockaddr_in lInfoServAddr;
        // service channel
        emissionSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (emissionSocket < 0) {
            fprintf(stderr, "ERROR opening socket");
            return(false);
        }
        // to allow address reuse (in case of of two close execution.
        int option = 1;
        setsockopt(emissionSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
        
        // clear buffer
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
    
    /**
     * Emit one.
     **/
    virtual bool send(const T* pMsg) {
        
        mutex.lock();
        
        if (connexionSocketList.size() == 0) {
            NHOFILE_LOG(logERROR) << "NHOTemplateFullDuplexConnectedEmitter::send no data socket." << std::endl;
            mutex.unlock();
            return(false);
        }
        
        if (pMsg == NULL) {
            NHOFILE_LOG(logERROR) << "NHOTemplateFullDuplexConnectedEmitter::send parameter null." << std::endl;
            mutex.unlock();
            return(false);
        }
        
        // send message
        for (int connexionSocket : connexionSocketList) {
            size_t lWrittenBytes = write(connexionSocket, pMsg->getData(), pMsg->getSize());
            if (lWrittenBytes != pMsg->getSize()) {
                NHOFILE_LOG(logERROR) << "NHOTemplateFullDuplexConnectedEmitter::send " << strerror(errno) << std::endl;
                NHOFILE_LOG(logERROR) << "NHOTemplateFullDuplexConnectedEmitter::send (number of written bytes:" <<
                lWrittenBytes << "/" << pMsg->getSize() << "))." << std::endl;
                mutex.unlock();
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

        }
        
        mutex.unlock();
        return(true);
        
    }

protected:
    std::thread* connectionThread;
    std::list<int>  connexionSocketList;
    unsigned short emissionPort;
    int emissionSocket;
    bool keepGoing;
    std::mutex mutex; // to prevent too close send calls
    
    ////////////////////////////////
    // Never ending loop.
    // Wait and register a connexion on the main socket.
    // This socket is the exit for captured images.
    virtual bool waitForConnectionOnSocket(){
        
        NHOFILE_LOG(logDEBUG) << "NHOTemplateFullDuplexConnectedEmitter::waitForConnectionOnDataSocket \n";
        
        socklen_t clilen;
        struct sockaddr_in cli_addr;
        
        listen(emissionSocket, 5);
        clilen = sizeof(cli_addr);
        
        while (keepGoing) {
            int connexionSocket = accept( emissionSocket,
                                     (struct sockaddr *) &cli_addr,
                                     &clilen);
            if (connexionSocket < 0) {
                // to avoid confusing message
                if (keepGoing == true) {
                    NHOFILE_LOG(logERROR) << "NHOTemplateFullDuplexConnectedEmitter::waitForConnectionOnDataSocket end of loop\n";
                }
                else {
                    NHOFILE_LOG(logDEBUG) << "NHOTemplateFullDuplexConnectedEmitter::waitForConnectionOnDataSocket error on accept\n";
                }
                return(false);
            }
            // Set the socket I/O mode: In this case FIONBIO
            // enables or disables the blocking mode for the
            // socket based on the numerical value of iMode.
            // If iMode = 0, blocking is enabled;
            // If iMode != 0, non-blocking mode is enabled.
            int lMode = 0;
// TEMPO            ioctl(emissionSocket, FIONBIO, &lMode);
            ioctl(connexionSocket, FIONBIO, &lMode);
            connexionSocketList.push_back(connexionSocket);
            // an attempt to flush socket
            //        int flag = 1;
            //        setsockopt(dataClientSocket, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));
            NHOFILE_LOG(logDEBUG) << "NHOTemplateFullDuplexConnectedEmitter::waitForConnectionOnDataSocket another connection\n";
        }
        
        NHOFILE_LOG(logDEBUG) << "NHOTemplateFullDuplexConnectedEmitter::waitForConnectionOnDataSocket End\n";
        
        return(true);
        
    }
    
};
#endif /* NHOFullDuplexConnectedEmitter_hpp */
