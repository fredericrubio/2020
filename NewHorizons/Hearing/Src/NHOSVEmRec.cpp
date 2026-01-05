//
//  NHOSVBroadcast.cpp
//  Hearing
//
//  Created by Frédéric Rubio on 14/09/2025.
//  Copyright © 2025 Frédéric Rubio. All rights reserved.
//

#include <errno.h>

#include "NHOSVEmRec.hpp"

/**
 *
 **/
NHOSVEmRec::NHOSVEmRec(int pPort) :
NHOTemplateBroadcastReceiver<NHOSolenoidValveMessage>(pPort) {
    
}

/**
 *
 **/
NHOSVEmRec::~NHOSVEmRec() {
    
}

/**
 * Emit one.
 **/
bool NHOSVEmRec::send(const NHOMessage *  pMsg) const {
    if (!receptionSocket) {
        NHOFILE_LOG(logERROR) << "NHOSVBroadcast::send: socket not initialized." << std::endl;
        return false;
    }
    if ((*pMsg).getSize() > 65507) {
        NHOFILE_LOG(logERROR) << "NHOSVBroadcast::send: message size exceeds limit." << std::endl;
        return false;
    }
    
    // configure for emission
    struct sockaddr_in lInfoServAddr;
    struct hostent* he = gethostbyname("255.255.255.255");
    // broadcast => .255 in the following address
    //    struct hostent* he = gethostbyname("192.168.0.255");
    //    192.168.1.32
    bzero((char *) &lInfoServAddr, sizeof(lInfoServAddr));
    lInfoServAddr.sin_family = AF_INET;
    //    lInfoServAddr.sin_addr.s_addr = INADDR_ANY; // INADDR_BROADCAST //? sure about that ?
    lInfoServAddr.sin_addr = *((struct in_addr *)he->h_addr);
    lInfoServAddr.sin_port = htons(receptionSocket);
    socklen_t optlen = sizeof(lInfoServAddr);
    
    // send message
    size_t lWrittenBytes = 0;
    lWrittenBytes = sendto(receptionSocket,
                           pMsg->getData(),
                           pMsg->getSize(),
                           0,
                           (struct sockaddr *)&lInfoServAddr,
                           optlen);
    if (lWrittenBytes != pMsg->getSize()) {
        NHOFILE_LOG(logERROR) << "NHOSVBroadcast::send: Sendig message failed <" << lWrittenBytes << ">"
        << " vs <" << pMsg->getSize() << ">" << std::endl;
        NHOFILE_LOG(logERROR) << "NHOSVBroadcast::send: Sendig message failed <" << errno << ">" << std::endl;
        return(false);
    }
    
    NHOFILE_LOG(logINFO) << "NHOSVBroadcast::send: Sent.\n";
    //delete pMsg;
    return(true);
    
}

/**
 * Emit one.
 **/
bool NHOSVEmRec::send(const sockaddr*  pAddress, const  NHOMessage  *pMsg) const {
    if (!receptionSocket) {
        NHOFILE_LOG(logERROR) << "NHOSVBroadcast::send: socket not initialized." << std::endl;
        return false;
    }
    if ((*pMsg).getSize() > 65507) {
        NHOFILE_LOG(logERROR) << "NHOSVBroadcast::send: message size exceeds limit." << std::endl;
        return false;
    }
    
    socklen_t optlen = sizeof(*pAddress);
    
    // send message
    size_t lWrittenBytes = 0;
    lWrittenBytes = sendto(receptionSocket,
                           pMsg->getData(),
                           pMsg->getSize(),
                           0,
                           pAddress,
                           optlen);
    if (lWrittenBytes != pMsg->getSize()) {
        NHOFILE_LOG(logERROR) << "NHOSVBroadcast::send: Sendig message failed <" << lWrittenBytes << ">"
        << " vs <" << pMsg->getSize() << ">" << std::endl;
        NHOFILE_LOG(logERROR) << "NHOSVBroadcast::send: Sendig message failed <" << errno << ">" << std::endl;
        return(false);
    }
    
    NHOFILE_LOG(logINFO) << "NHOSVBroadcast::send: Sent.\n";
    //delete pMsg;
    return(true);
}

