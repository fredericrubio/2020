//
//  NHOBroadCastElitter.cpp
//  Network
//
//  Created by Frédéric Rubio on 12/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "NHOMessage.hpp"
#include "NHOLOG.hpp"
#include "NHOBroadcastEmitter.hpp"

/**
 * Constructor
 **/
NHOBroadcastEmitter::NHOBroadcastEmitter(const unsigned short pPort, const unsigned short pPeriod)
: NHOEmitter(pPort, pPeriod) {
}

/**
 * launch the thread dedicaded to connexion
 **/
bool NHOBroadcastEmitter::initiate() {

    NHOFILE_LOG(logDEBUG) << "NHOBroadcastEmitter::initiate: " << std::endl;
    
    int broadcast = 1;
    
    //    char broadcast = '1';
    socklen_t optlen = sizeof(broadcast);

    // service channel
    emissionSocket = socket(AF_INET, SOCK_DGRAM, 0);//IPPROTO_UDP);
    if (emissionSocket < 0) {
        NHOFILE_LOG(logERROR) << "HEM_Server::initiate: Unable to create emission socket" << std::endl;
        return(false);
    }
    // this call is what allows broadcast packets to be sent:
    /*    getsockopt( emissionSocket, SOL_SOCKET, SO_BROADCAST,
     &broadcast, &optlen);
     */    if (setsockopt( emissionSocket, SOL_SOCKET, SO_BROADCAST,
                          &broadcast, optlen) == -1) {
         NHOFILE_LOG(logERROR) << "HEM_Server::initiate: " << strerror(errno) << std::endl;
         NHOFILE_LOG(logERROR) << "HEM_Server::initiate: setsockopt (SO_BROADCAST)" << std::endl;
         return(false);
     }

    return(true);
    
}

/**
 * Emit one.
 **/
bool NHOBroadcastEmitter::send(const NHOMessage *  pMsg) const {
    if (!emissionSocket) {
        NHOFILE_LOG(logERROR) << "NHOBroadcastEmitter::send: socket not initialized." << std::endl;
        return false;
    }
    if ((*pMsg).getSize() > 65507) {
        NHOFILE_LOG(logERROR) << "NHOBroadcastEmitter::send: message size exceeds limit." << std::endl;
        return false;
    }
    // configure for emission
    struct sockaddr_in lInfoServAddr;
    //    struct hostent* he = gethostbyname("255.255.255.255");
    // broadcast => .255 in the following address
    struct hostent* he = gethostbyname("192.168.0.255");
    
    bzero((char *) &lInfoServAddr, sizeof(lInfoServAddr));
    lInfoServAddr.sin_family = AF_INET;
    //    lInfoServAddr.sin_addr.s_addr = INADDR_ANY; // INADDR_BROADCAST //? sure about that ?
    lInfoServAddr.sin_addr = *((struct in_addr *)he->h_addr);
    lInfoServAddr.sin_port = htons(emissionPort);
    
    socklen_t optlen = sizeof(lInfoServAddr);
    
    // send message
    size_t lWrittenBytes = 0;
    lWrittenBytes = sendto(emissionSocket,
                           pMsg->getData(),
                           pMsg->getSize(),
                           0,
                           (struct sockaddr *)&lInfoServAddr,
                           optlen);
    if (lWrittenBytes != pMsg->getSize()) {
        NHOFILE_LOG(logERROR) << "NHOEmitter::send: Sendig message failed <" << lWrittenBytes << ">"
        << " vs <" << pMsg->getSize() << ">" << std::endl;
        return(false);
    }
    
    delete pMsg;
    
    return(true);
    
}
