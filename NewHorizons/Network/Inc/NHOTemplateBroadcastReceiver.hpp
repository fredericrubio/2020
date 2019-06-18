//
//  NHOTemplateBroadcastReceiver.hpp
//  Network
//
//  Created by Frédéric Rubio on 13/06/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOTemplateBroadcastReceiver_hpp
#define NHOTemplateBroadcastReceiver_hpp

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
#include "NHOMessage.hpp"
#include "NHOLOG.hpp"
#include "NHOAckMessage.hpp"

template <class T>
class NHOTemplateBroadcastReceiver: public NHOSubject<T> {
    
public:
    NHOTemplateBroadcastReceiver(const int pConnexionPort) {
        this->port = pConnexionPort;
        this->message = new T(clock());
        this->keepGoing = true;
        this->observers.clear();
    }
    
    /**
     * Initialize network stuff.
     **/
    bool initiate(){
        
        // local variables
        struct addrinfo hints, *servinfo, *p;
        int rv;
        //    int broadcast = 1;
        //    socklen_t optlen = sizeof(broadcast);
        
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_INET;//AF_UNSPEC; // set to AF_INET to force IPv4
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_flags = AI_PASSIVE; // use my IP
        
        if ((rv = getaddrinfo(NULL, std::to_string(this->port).c_str(), &hints, &servinfo)) != 0) {
            NHOFILE_LOG(logERROR) << "NHOTemplateBroadcastReceiver::initiate getaddrinfo:" << gai_strerror(rv) << std::endl;
            return false;
        }
        
        // loop through all the results and bind to the first we can
        for (p = servinfo; p != NULL; p = p->ai_next) {
            if ((this->recptionSocket = socket(p->ai_family, p->ai_socktype,
                                     p->ai_protocol)) == -1) {
                continue;
            }
            
            if (bind(this->recptionSocket, p->ai_addr, p->ai_addrlen) == -1) {
                close(this->recptionSocket);
                continue;
            }
            break;
        }
        
        if (p == NULL) {
            NHOFILE_LOG(logERROR) << "NHOTemplateBroadcastReceiver::initiate failed to bind socket." << std::endl;
            return false;
        }
        this->thread = new std::thread(&NHOTemplateBroadcastReceiver::receive, std::ref(*this));
        return true;
    }
    
    /**
     * Terminte reception loop and close socket.
     **/
    bool terminate() {
        
        this->keepGoing = false;
        close(this->recptionSocket);
        return true;
        
    }
    
    /**
     * Receive
     **/
    bool receive(){
        
        NHOFILE_LOG(logDEBUG) << "NHOTemplateBroadcastReceiver::receive \n";
        
        long numbytes;
        struct sockaddr_storage their_addr;
this->mutex.lock();
        int size = this->message->getSize();
this->mutex.unlock();
        char* buf = new char[size];
        socklen_t addr_len;
        
        /*    char lBuffer[IMP_Message::MAX_SIZE];
         long lReceivedBytes;
         IMP_Message* lMessage = NULL;
         */
//        NHOHEMMessage* lMessage = new NHOHEMMessage(clock());
        
        while (this->keepGoing) {
            
        NHOFILE_LOG(logDEBUG) << "NHOTemplateBroadcastReceiver::receive stalled on recvfrom\n";

            addr_len = sizeof their_addr;
            if ((numbytes = recvfrom(this->recptionSocket, buf, size-1 , 0,
                                     (struct sockaddr *)&their_addr, &addr_len)) == -1) {
                std::cout << "NHOTemplateBroadcastReceiver::receive recvfrom\n";
                return(false);
            }
this->mutex.lock();
            this->message->setData((int) numbytes, buf);
            this->message->unserialize();
this->mutex.unlock();
            NHOFILE_LOG(logDEBUG) << "NHOTemplateBroadcastReceiver::receive\n";
            //        NHOFILE_LOG(logDEBUG) << "NHOHEMStorageUnit::receiveHEM CPU:" << lMessage->getHEMData()->getCPUUsage() << std::endl;
            //        NHOFILE_LOG(logDEBUG) << "NHOHEMStorageUnit::receiveHEM Memory:" << lMessage->getHEMData()->getMemoryUsage() << std::endl;
            //        NHOFILE_LOG(logDEBUG) << "NHOHEMStorageUnit::receiveHEM Temperature:" << lMessage->getHEMData()->getTemperature() << std::endl;
            //        NHOFILE_LOG(logDEBUG) << "NHOHEMStorageUnit::receiveHEM Modes: " ;
            //        for (int loop = 0 ; loop < NHOWiringPi::GPIO_PINS ; loop++) {
            //            NHOFILE_LOG(logDEBUG) << lMessage->getHEMData()->getPinModes()[loop] << " " ;
            //        }
            //            NHOFILE_LOG(logDEBUG) << std::endl;
        }
        
        NHOFILE_LOG(logDEBUG) << "NHOHEMStorageUnit::receiveImageMessage End\n";
        return true;
    }
   
protected:
    unsigned int port;
    int recptionSocket;
    std::thread* thread;
    std::mutex mutex;
    T* message;
    bool keepGoing;

};
#endif /* NHOTemplateBroadcastReceiver_hpp */
