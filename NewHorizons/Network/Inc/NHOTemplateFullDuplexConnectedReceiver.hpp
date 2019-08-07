//
//  NHOImageStorageUnit.hpp
//  StorageUnit
//
//  Created by Frédéric Rubio on 28/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOImageStorageUnit_hpp
#define NHOImageStorageUnit_hpp

#include <string>
#include <thread>
#include <stdio.h>
#include <iostream>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sys/ioctl.h>
#include <netinet/tcp.h>
#include <mutex>

#include "NHOSubject.hpp"
#include "NHOCameraData.hpp"
#include "NHOImage.hpp"
#include "NHOMessage.hpp"
#include "NHOLOG.hpp"
#include "NHOAckMessage.hpp"

template <class T>
class NHOTemplateFullDuplexConnectedReceiver: public NHOSubject<T> {
    
    public :
    NHOTemplateFullDuplexConnectedReceiver(const std::string pHostName,
                                           const int pConnexionPort) {
        hostName = pHostName;
        connexionPort = pConnexionPort;
        message = new T(clock());
        
        this->observers.clear();
    }
    
    bool initiate(){
        
        // local variables
        struct hostent *lServer;
        
        lServer = gethostbyname(hostName.c_str());
        if (lServer == NULL) {
            NHOFILE_LOG(logERROR) << "NHOTemplateFullDuplexConnectedReceiver::initiate ERROR, no such host"  << std::endl;
            return false;
        }
        ///////////////////
        // connection
        struct sockaddr_in lServAddr;
        connexionSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (connexionSocket < 0) {
            NHOFILE_LOG(logERROR) << "NHOTemplateFullDuplexConnectedReceiver::initiate ERROR opening data socket" << std::endl;
            return false;
        }
        
        bzero((char *) &lServAddr, sizeof(lServAddr));
        lServAddr.sin_family = AF_INET;
        bcopy((char *)lServer->h_addr,
              (char *)&lServAddr.sin_addr.s_addr,
              lServer->h_length);
        lServAddr.sin_port = htons(connexionPort);
        if (connect(connexionSocket, (struct sockaddr *) &lServAddr, sizeof(lServAddr)) < 0) {
            NHOFILE_LOG(logERROR) << "NHOTemplateFullDuplexConnectedReceiver::initiate ERROR connecting data port "  << strerror(errno) << std::endl;
            return false;
        }
        // Set the socket I/O mode: In this case FIONBIO
        // enables or disables the blocking mode for the
        // socket based on the numerical value of iMode.
        // If iMode = 0, blocking is enabled;
        // If iMode != 0, non-blocking mode is enabled.
        int lMode = 0;
        ioctl(connexionSocket, FIONBIO, &lMode);
        
        // an attempt to flush socket
        int flag = 1;
        setsockopt(connexionSocket, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));
        
        connexionThread = new std::thread(&NHOTemplateFullDuplexConnectedReceiver<T>::receive, std::ref(*this));
        
        return true;
    }
    
    
    /**
     * To terminate safely.
     **/
    bool terminate() {
        
        close(connexionSocket);
        return true;
        
    }

    /**
     * Receive
     **/
    bool receive(){
        
        NHOFILE_LOG(logDEBUG) << "NHOTemplateFullDuplexConnectedReceiver::receive sart reception loop" << std::endl;
        
        // waiting for the end of initialization through service messages
        usleep(1000);
        
messageMutex.lock();
        unsigned int lSize = message->computeSize();
messageMutex.unlock();
        unsigned char* lBuffer = NULL;
        long lReceivedBytes;
        unsigned long lNbBytes;
        unsigned int lTotalOfBytes;
        lBuffer = (unsigned char *) calloc(lSize, sizeof(char));
        while (1) {
            lNbBytes = 0;
            lReceivedBytes = 0;
            lTotalOfBytes = 0;
            // wait for an answer
            bzero(lBuffer, lSize);
            while (lNbBytes < lSize) {
                lReceivedBytes = read(connexionSocket, lBuffer + lNbBytes, lSize);
                if (lReceivedBytes < 0) {
                    // break the reception loop
                    lNbBytes = lSize * 2;
                }
                lNbBytes += lReceivedBytes;
                lTotalOfBytes += lReceivedBytes;
            }
            
            if (lNbBytes > lSize) {
                NHOFILE_LOG(logERROR) << "ERROR NHOTemplateFullDuplexConnectedReceiver::receive message lost." << std::endl;
            }
            else {
messageMutex.lock();
                message->setData((const char*) lBuffer);
                message->unserialize();
messageMutex.unlock();
                NHOFILE_LOG(logDEBUG) << "NHOTemplateFullDuplexConnectedReceiver::receive new reception." << std::endl;
            }
            
messageMutex.lock();
            this->setVal(message);
            this->notify();
messageMutex.unlock();

            // send an aknownledgement
            NHOAckMessage* lAckMsg = new NHOAckMessage(clock(), lTotalOfBytes);
            lAckMsg->serialize();
            ssize_t lWrittenBytes = write(connexionSocket, lAckMsg->getData(), lAckMsg->getSize());
            if (lWrittenBytes < 0) {
                std::cout << "ERROR NHOTemplateFullDuplexConnectedReceiver::receive" << std::endl;
                delete lAckMsg;
                return(false);
            }
            delete lAckMsg;
        }
        
#ifdef _DEBUG
        std::cout << "NHOTemplateFullDuplexConnectedReceiver::receive End\n";
#endif
        return true;
    }
    
protected:
    /**
     * Connexion stuff
     **/
    unsigned int connexionPort;
    std::string hostName;
    int connexionSocket;
    std::thread* connexionThread;
    
    T* message;
    
    // Concurrent data acccess management
    std::mutex messageMutex;
    
};

#endif /* NHOImageStorageUnit_hpp */
