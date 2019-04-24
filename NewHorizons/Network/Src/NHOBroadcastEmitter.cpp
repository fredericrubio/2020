//
//  NHOBroadCastElitter.cpp
//  Network
//
//  Created by Frédéric Rubio on 12/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//
#include "NHOLOG.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "NHOBroadCastEmitter.hpp"

/**
 * Constructor
 **/
NHOBroadcastEmitter::NHOBroadcastEmitter(const unsigned short pPort, const unsigned short pPeriod)
: NHOEmitter(pPort, pPeriod)
{
}

/**
 * launch the thread dedicaded to connexion
 **/
bool NHOBroadcastEmitter::initiate() {

    NHOFILE_LOG(logDEBUG) << "NHOBroadcastEmitter::initiate: " << std::endl;
    
//    struct sockaddr_in lInfoServAddr;
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
