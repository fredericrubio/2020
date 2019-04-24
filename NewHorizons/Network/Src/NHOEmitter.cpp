//
//  NHOEmitter.cpp
//  Network
//
//  Created by Frédéric Rubio on 10/04/2019.
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

#include "NHOLOG.hpp"
#include "NHOMessage.hpp"
#include "NHOMessageFactory.hpp"
#include "NHOMessage.hpp"

#include "NHOEmitter.hpp"
/**
 * Member functions
 **/
NHOEmitter::NHOEmitter(const unsigned short pPort, const unsigned short pPeriod) {
    
    port = pPort;
    sampling = pPeriod;
    counter = 0;
    
}


/**
 * Emit one.
 **/
bool NHOEmitter::send(const NHOMessage* pMsg) const {
    
    // configure for emission
    struct sockaddr_in lInfoServAddr;
    //    struct hostent* he = gethostbyname("255.255.255.255");
    // broadcast => .255 in the following address
    struct hostent* he = gethostbyname("192.168.0.255");
    
    bzero((char *) &lInfoServAddr, sizeof(lInfoServAddr));
    lInfoServAddr.sin_family = AF_INET;
    //    lInfoServAddr.sin_addr.s_addr = INADDR_ANY; // INADDR_BROADCAST //? sure about that ?
    lInfoServAddr.sin_addr = *((struct in_addr *)he->h_addr);
    lInfoServAddr.sin_port = htons(port);
    
    socklen_t optlen = sizeof(lInfoServAddr);
    
    // send message
    size_t lWrittenBytes = sendto(emissionSocket,
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

    return(true);
    
}


