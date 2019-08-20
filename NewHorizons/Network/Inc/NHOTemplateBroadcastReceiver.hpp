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
            this->receptionSocket = socket(p->ai_family,
                                          p->ai_socktype,
                                          p->ai_protocol);
            if (this->receptionSocket == -1) {
                NHOFILE_LOG(logERROR) << "NHOTemplateBroadcastReceiver::initiate socket (socket):" << strerror(errno) << std::endl;
                continue;
            }
            // to allow address reuse (in case of of two close execution.
            int option = 1;
            if (setsockopt(this->receptionSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) == -1) {
                NHOFILE_LOG(logERROR) << "NHOTemplateBroadcastReceiver::initiate socket (SO_REUSEADDR):" << strerror(errno) << std::endl;
                continue;
            }
            option = 1;
            if (setsockopt(this->receptionSocket, SOL_SOCKET, SO_REUSEPORT, &option, sizeof(option)) == -1) {
                NHOFILE_LOG(logERROR) << "NHOTemplateBroadcastReceiver::initiate socket (SO_REUSEPORT):" << strerror(errno) << std::endl;
                continue;
            }
            // a security based on time
            struct timeval timeout;
            timeout.tv_sec = 100;
            timeout.tv_usec = 0;
            if (setsockopt (this->receptionSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
                            sizeof(timeout)) < 0) {
                NHOFILE_LOG(logERROR) << "NHOTemplateBroadcastReceiver::initiate socket (SO_RCVTIMEO):" << strerror(errno) << std::endl;
                continue;
            }
            
            if (bind(this->receptionSocket, p->ai_addr, p->ai_addrlen) == -1) {
                NHOFILE_LOG(logERROR) << "NHOTemplateBroadcastReceiver::initiate bind:" << strerror(errno) << std::endl;
                close(this->receptionSocket);
                continue;
            }
            
            linger lin;
            lin.l_onoff = 1;
            lin.l_linger = 0;
            if (setsockopt(this->receptionSocket,
                           SOL_SOCKET,
                           SO_LINGER,
                           (const char *)&lin,
                           sizeof(lin)) == -1) {
                NHOFILE_LOG(logERROR) << "NHOTemplateBroadcastReceiver::initiate socket:" << strerror(errno) << std::endl;
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
        return (close(this->receptionSocket) ==0);
        
    }
    
    /**
     * Receive
     **/
    bool receive(){
        
        NHOFILE_LOG(logDEBUG) << "NHOTemplateBroadcastReceiver::receive \n";
        
        long numbytes;
        struct sockaddr_storage their_addr;
this->mutex.lock();
        unsigned long size = this->message->getHEMData()->getSize();
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
            if ((numbytes = recvfrom(this->receptionSocket, buf, size , 0,
                                     (struct sockaddr *)&their_addr, &addr_len)) == -1) {
                std::cout << "NHOTemplateBroadcastReceiver::receive recvfrom error " << strerror(errno) << "\n";
                return(false);
            }
            std::cout << "1\n";
this->mutex.lock();
            this->message->setData((int) numbytes, buf);
            this->message->unserialize();
            this->setVal(this->message);
            this->notify();
this->mutex.unlock();
//            le changement de couleurs des diodes est lent : fréquence d'envoi, fréquence de rafraîchissement de la GUI ?
//            les diodes allumées ne retombent pas après la commande "STOP"
//            on a la même configuration de diodes pour des commades différents : "LEFT" et "FORWARD" par exemple
//            il faut d'abord lancer l'interface graphique PUIS le rover.
            NHOFILE_LOG(logDEBUG) << "NHOTemplateBroadcastReceiver::receive\n";
        }
        
        NHOFILE_LOG(logDEBUG) << "NHOHEMStorageUnit::receiveImageMessage End\n";
        return true;
    }
   
protected:
    unsigned int port;
    int receptionSocket;
    std::thread* thread;
    std::mutex mutex;
    T* message;
    bool keepGoing;

};
#endif /* NHOTemplateBroadcastReceiver_hpp */
