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
#include "NHOImageSizeMessage.hpp"
#include "NHOImageStorageUnit.hpp"
#include "NHOMessage.hpp"
#include "NHOAckMessage.hpp"
#include "NHOCameraDataMessage.hpp"

NHOImageStorageUnit::NHOImageStorageUnit(const std::string pHostName, const int pDataPort, const int pInfoPort) :
hostName(pHostName), dataPort(pDataPort), infoPort(pInfoPort), cameraData(0) {
    
}

bool NHOImageStorageUnit::initiate() {
    
    // local variables
    struct hostent *server;
    
    server = gethostbyname(hostName.c_str());
    if (server == NULL) {
        NHOFILE_LOG(logERROR) << "NHPImageStorageUnit::initiate ERROR, no such host"  << std::endl;
        return false;
    }

    struct sockaddr_in lInfoServAddr;
    infoSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (infoSocket < 0) {
        NHOFILE_LOG(logERROR) << "NHPImageStorageUnit::initiate ERROR opening socket" << std::endl;
        return false;
    }
    // to allow address reuse (in case of of too close execution).
    int option = 1;
    setsockopt(infoSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    bzero((char *) &lInfoServAddr, sizeof(lInfoServAddr));
    lInfoServAddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&lInfoServAddr.sin_addr.s_addr,
          server->h_length);
    lInfoServAddr.sin_port = htons(infoPort);
    if (connect(infoSocket, (struct sockaddr *) &lInfoServAddr, sizeof(lInfoServAddr)) < 0) {
        NHOFILE_LOG(logERROR) << "NHPImageStorageUnit::initiate " << strerror(errno) ;
        NHOFILE_LOG(logDEBUG) << "NHPImageStorageUnit::initiate ERROR connecting info port"  << std::endl;
        return false;
    }

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

    infoThread = new std::thread(&NHOImageStorageUnit::receiveInfoMessage, std::ref(*this));
    dataThread = NULL;

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

    // waiting for the end of initialization through service messages
    usleep(1000);
     
    // the sizes of transferred images is known:
    NHOCameraDataMessage* lCameraDataMessage = new NHOCameraDataMessage(clock());
    lCameraDataMessage->setCameraData(new NHOCameraData(0));
    NHOImage* lImage = new NHOImage();
    lImage->setWidth(cameraData.getImage()->getWidth());
    lImage->setHeight(cameraData.getImage()->getHeight());
    lImage->setFormat(cameraData.getImage()->getFormat());
    // TO DO: replace '3' by a real estimation
    lImage->setPixels(lImage->getWidth() * lImage->getHeight() * 3, NULL);
    lCameraDataMessage->getCameraData()->setImage(lImage);
    unsigned int lSize = lCameraDataMessage->computeSize();
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
            lReceivedBytes = read(dataSocket, lBuffer + lNbBytes, lSize);
            if (lReceivedBytes < 0) {
                // break the reception loop
                lNbBytes = lSize * 2;
            }
            lNbBytes += lReceivedBytes;
            lTotalOfBytes += lReceivedBytes;
        }
        
        if (lNbBytes > lSize) {
            NHOFILE_LOG(logERROR) << "ERROR IMP_Client::receiveImageMessage image lost." << std::endl;
        }
        else {
            lCameraDataMessage->setData((char *)lBuffer);
            lCameraDataMessage->unserialize();
            cameraData.getImage()->setPixels(cameraData.getImage()->getDataSize(),
                                             (unsigned char *)lCameraDataMessage->getCameraData()->getImage()->getPixels());
            NHOFILE_LOG(logDEBUG) << "NHOImageStorageUnit::receiveImageMessage another image." << std::endl;
            cameraData.getImage()->saveToDisk();
            /*            std::string lFileName = std::to_string(clock()) + "_image.ppm";
             std::ofstream outFile ( lFileName ,std::ios::binary );
             outFile<<"P6\n" << image.getWidth() << " " << image.getHeight() << " 255\n";
             outFile.write ( ( char* ) image.getPixels(), image.getDataSize());
             outFile.close();*/
        }
        
        // send an aknownledgement
        NHOAckMessage* lAckMsg = new NHOAckMessage(clock(), lTotalOfBytes);
        lAckMsg->serialize();
        ssize_t lWrittenBytes = write(dataSocket, lAckMsg->getData(), lAckMsg->getSize());
        if (lWrittenBytes < 0) {
            std::cout << "ERROR IMP_Client::receiveImageMessage" << std::endl;
            delete lAckMsg;
            return(false);
        }
        delete lAckMsg;
    }

#ifdef _DEBUG
    std::cout << "IMP_Client::receiveImageMessage End\n";
#endif
    return true;
}

/////////////////////////////////////
// Never ending loop
// Wait for service messages in the dedicated socket
bool NHOImageStorageUnit::receiveInfoMessage() {

    NHOFILE_LOG(logDEBUG) << "NHOImageStorageUnit::receiveInfoMessage" << std::endl;
    
    char lBuffer[NHOMessage::MAX_SIZE];
    long lReceivedBytes;
    NHOImageSizeMessage* lMessage = NULL;
    
    while (1) {
        // wait for an answer
        bzero(lBuffer, sizeof(unsigned int));
        lReceivedBytes = read(infoSocket, lBuffer, NHOMessage::MAX_SIZE);
        if (lReceivedBytes < 0) {
            std::cout << "ERROR NHOImageStorageUnit::receiveInfoMessage" << std::endl;
            return(false);
        }
        
        lMessage = NHOMessageFactory::build(lBuffer);
        if (lMessage) {
            NHOImage* lImage = new NHOImage();
            
            lImage->setWidth(lMessage->getWidth());
            lImage->setHeight(lMessage->getHeight());
            lImage->setFormat(lMessage->getFormat());
            
            cameraData.setImage(lImage);
            
            // send an aknownledgement
            NHOAckMessage* lAckMsg = new NHOAckMessage(clock(), (int) lReceivedBytes);
            lAckMsg->serialize();
            ssize_t lWrittenBytes = write(infoSocket, lAckMsg->getData(), lAckMsg->getSize());
            if (lWrittenBytes < 0) {
                delete lAckMsg;
                std::cout << "ERROR NHOImageStorageUnit::receiveInfoMessage" << std::endl;
                return(false);
            }
            delete lAckMsg;

            if ((dataSocket > 0)
                &&
                (dataThread == NULL)) {
                dataThread = new std::thread(&NHOImageStorageUnit::receiveImageMessage, std::ref(*this));
            }
            
            // delete last received message
            lMessage->setData(NULL);
            delete lMessage;
#ifdef _DEBUG
            std::cout << "NHOImageStorageUnit::receiveInfoMessage image initialized" << std::endl;
#endif
        }

    }
    
#ifdef _DEBUG
    std::cout << "NHOImageStorageUnit::receiveInfoMessage End\n";
#endif
    return true;
}

