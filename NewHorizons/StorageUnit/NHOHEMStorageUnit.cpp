//
//  NHOImageStorageUnit.cpp
//  StorageUnit
//
//  Created by Frédéric Rubio on 28/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

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

#include "NHOLOG.hpp"
#include "NHOAckMessage.hpp"
#include "NHOHEMStorageUnit.hpp"
#include "NHOMessage.hpp"
#include "NHOAckMessage.hpp"
#include "NHOHEMMessage.hpp"
#include "NHOHEMData.hpp"

static const size_t sMAX_BUFFER_SIZE = NHOHEMData::getSize();

NHOHEMStorageUnit::NHOHEMStorageUnit(const std::string pHostName, const int pDataPort) :
hostName(pHostName), dataPort(pDataPort) {
    
}

bool NHOHEMStorageUnit::initiate() {
    
    // local variables
    struct addrinfo hints, *servinfo, *p;
    int rv;
//    int broadcast = 1;
//    socklen_t optlen = sizeof(broadcast);
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;//AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP
    
    if ((rv = getaddrinfo(NULL, std::to_string(dataPort).c_str(), &hints, &servinfo)) != 0) {
        NHOFILE_LOG(logERROR) << "NHOHEMStorageUnit::initiate getaddrinfo:" << gai_strerror(rv) << std::endl;
        return 1;
    }
    
    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((dataSocket = socket(p->ai_family, p->ai_socktype,
                                 p->ai_protocol)) == -1) {
            continue;
        }
        // to allow address reuse (in case of of two close execution.
        int option = 1;
        if (setsockopt(dataSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) == -1) {
            NHOFILE_LOG(logERROR) << "NHOHEMStorageUnit::initiate setsockopt:" << strerror(errno) << std::endl;
            continue;
        }
        option = 1;
        if (setsockopt(dataSocket, SOL_SOCKET, SO_REUSEPORT, &option, sizeof(option)) == -1) {
            NHOFILE_LOG(logERROR) << "NHOHEMStorageUnit::initiate setsockopt:" << strerror(errno) << std::endl;
            continue;
        }
        if (bind(dataSocket, p->ai_addr, p->ai_addrlen) == -1) {
            close(dataSocket);
            continue;
        }
        break;
    }
    
    if (p == NULL) {
        NHOFILE_LOG(logERROR) << "NHOHEMStorageUnit::initiate failed to bind socket." << std::endl;
        return 2;
    }
    dataThread = new std::thread(&NHOHEMStorageUnit::receiveHEM, std::ref(*this));
    return true;
}

bool NHOHEMStorageUnit::terminate() {
    
    return true;
    
}

/////////////////////////////////////
// Never ending loop
// Wait for data messages on the dedicated socket
bool NHOHEMStorageUnit::receiveHEM() {

    NHOFILE_LOG(logDEBUG) << "NHOHEMStorageUnit::receiveImageMessage" << std::endl;

    long numbytes;
    struct sockaddr_storage their_addr;
    char buf[sMAX_BUFFER_SIZE];
    socklen_t addr_len;
    
    /*    char lBuffer[IMP_Message::MAX_SIZE];
     long lReceivedBytes;
     IMP_Message* lMessage = NULL;
     */
//    NHOHEMMessage* lMessage = new NHOHEMMessage(clock());
    
    while (1) {
        
#ifdef _DEBUG
        std::cout << "HEM_Client::receiveDataMessage stalled on recvfrom" << std::endl;
#endif
        addr_len = sizeof their_addr;
        if ((numbytes = recvfrom(dataSocket, buf, sMAX_BUFFER_SIZE/*-1*/ , 0,
                                 (struct sockaddr *)&their_addr, &addr_len)) == -1) {
            std::cout << "HEM_Client::receiveDataMessage recvfrom" << std::endl;
            return(false);
        }
        // TO DO
//        lMessage = new NHOHEMMessage(clock());
//        lMessage->setData((int) numbytes, buf);
//        lMessage->unserialize();
        NHOFILE_LOG(logDEBUG) << "NHOHEMStorageUnit::receiveHEM HEM received.\n";
        //        NHOFILE_LOG(logDEBUG) << "NHOHEMStorageUnit::receiveHEM CPU:" << lMessage->getHEMData()->getCPUUsage() << std::endl;
//        NHOFILE_LOG(logDEBUG) << "NHOHEMStorageUnit::receiveHEM Memory:" << lMessage->getHEMData()->getMemoryUsage() << std::endl;
//        NHOFILE_LOG(logDEBUG) << "NHOHEMStorageUnit::receiveHEM Temperature:" << lMessage->getHEMData()->getTemperature() << std::endl;
//        NHOFILE_LOG(logDEBUG) << "NHOHEMStorageUnit::receiveHEM Modes: " ;
//        for (int loop = 0 ; loop < NHOWiringPi::GPIO_PINS ; loop++) {
//            NHOFILE_LOG(logDEBUG) << lMessage->getHEMData()->getPinModes()[loop] << " " ;
//        }
//            NHOFILE_LOG(logDEBUG) << std::endl;
//        delete lMessage;
    }
    
#ifdef _DEBUG
    std::cout << "NHOHEMStorageUnit::receiveImageMessage End\n";
#endif
    return true;
}
