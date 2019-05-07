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

#include "NHOImageStorageUnit.hpp"

NHOImageStorageUnit::NHOImageStorageUnit(const std::string pHostName, const int pDataPort) :
hostName(pHostName), dataPort(pDataPort), cmaeraData(0) {
    
}

bool NHOImageStorageUnit::initiate() {
    
    // local variables
    struct hostent *server;
    
    server = gethostbyname(hostName.c_str());
    if (server == NULL) {
        NHOFILE_LOG(logERROR) << "NHPImageStorageUnit::initiate ERROR, no such host"  << std::endl;
        return false;
    }
    
/*
    struct sockaddr_in lInfoServAddr;
    infoSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (infoSocket < 0) {
        NHOFILE_LOG(logERROR) << "NHPImageStorageUnit::initiate ERROR opening socket" << std::endl;
        return false;
    }
    
    bzero((char *) &lInfoServAddr, sizeof(lInfoServAddr));
    lInfoServAddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&lInfoServAddr.sin_addr.s_addr,
          server->h_length);
    lInfoServAddr.sin_port = htons(infoPort);
    if (connect(infoSocket, (struct sockaddr *) &lInfoServAddr, sizeof(lInfoServAddr)) < 0) {
        NHOFILE_LOG(logDEBUG) << "NHPImageStorageUnit::initiate ERROR connecting info port"  << std::endl;
        return false;
    }
 */
    ///////////////////
    // data connection
    struct sockaddr_in lDataServAddr;
    dataSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (dataSocket < 0) {
        NHOFILE_LOG(logERROR) << "NHPImageStorageUnit::initiate ERROR opening data socket" << std::endl;
        return false;
    }
    
    bzero((char *) &lDataServAddr, sizeof(lDataServAddr));
    lDataServAddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&lDataServAddr.sin_addr.s_addr,
          server->h_length);
    lDataServAddr.sin_port = htons(dataPort);
    if (connect(dataSocket, (struct sockaddr *) &lDataServAddr, sizeof(lDataServAddr)) < 0) {
        NHOFILE_LOG(logERROR) << "NHPImageStorageUnit::initiate ERROR connecting data port"  << std::endl;
        return false;
    }
    // Set the socket I/O mode: In this case FIONBIO
    // enables or disables the blocking mode for the
    // socket based on the numerical value of iMode.
    // If iMode = 0, blocking is enabled;
    // If iMode != 0, non-blocking mode is enabled.
    int lMode = 0;
    ioctl(dataSocket, FIONBIO, &lMode);
    
    // an attempt to flush socket
    int flag = 1;
    setsockopt(dataSocket, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));
    
    dataThread = new std::thread(&NHOImageStorageUnit::receiveImageMessage, std::ref(*this));

    return true;
}

bool NHOImageStorageUnit::terminate() {
    
    return true;
    
}

bool NHOImageStorageUnit::waitForConnection() {
    
    return true;
    
}

/////////////////////////////////////
// Never ending loop
// Wait for data messages on the dedicated socket
bool NHOImageStorageUnit::receiveImageMessage() {

    NHOFILE_LOG(logDEBUG) << "IMP_Client::receiveImageMessage" << std::endl;
#ifdef ZERO
    // waiting for the end of initialization through service messages
    usleep(1000);
    
    // the sizes of transferred images is known:
    unsigned char* lBuffer = NULL;
    long lReceivedBytes;
    unsigned long lNbBytes;
    unsigned int lTotalOfBytes;
    unsigned int lExpectedNumberOfReceivedBytes = 0;
    lBuffer = (unsigned char *) calloc(image.getDataSize(), sizeof(char));
    while (1) {
        lNbBytes = 0;
        lReceivedBytes = 0;
        lTotalOfBytes = 0;
        // wait for an answer
        bzero(lBuffer, image.getDataSize());
        while (lNbBytes < image.getDataSize()) {
            lReceivedBytes = read(dataSocket, lBuffer + lNbBytes, image.getDataSize());
            if (lReceivedBytes < 0) {
                lNbBytes = image.getDataSize() * 2;
            }
            lNbBytes += lReceivedBytes;
            lTotalOfBytes += lReceivedBytes;
        }
        
        if (lNbBytes > image.getDataSize()) {
            NHOFILE_LOG(logERROR) << "ERROR IMP_Client::receiveImageMessage image lost." << std::endl;
        }
        else {
            image.setPixels(image.getDataSize(), lBuffer);
            std::cout << "IMP_Client::receiveImageMessage another image " << lNbBytes << std::endl;
            /*            std::string lFileName = std::to_string(clock()) + "_image.ppm";
             std::ofstream outFile ( lFileName ,std::ios::binary );
             outFile<<"P6\n" << image.getWidth() << " " << image.getHeight() << " 255\n";
             outFile.write ( ( char* ) image.getPixels(), image.getDataSize());
             outFile.close();*/
        }
        
        // send an aknownledgement
        NHOAckMessage* lAckMsg = new NHOAckMessage(lTotalOfBytes);
        char* lMsgBuf = (char *) calloc(lAckMsg->getSize(), sizeof(char));
        lAckMsg->serialize(lMsgBuf);
        ssize_t lWrittenBytes = write(dataSocket, lMsgBuf, lAckMsg->getSize());
        if (lWrittenBytes < 0) {
            std::cout << "ERROR IMP_Client::receiveImageMessage" << std::endl;
            return(false);
        }
    }
#endif
#ifdef _DEBUG
    std::cout << "IMP_Client::receiveImageMessage End\n";
#endif
    return true;
}

