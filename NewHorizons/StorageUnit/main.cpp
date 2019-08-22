//
//  main.cpp
//  StorageUnit
//
//  Created by Frédéric Rubio on 28/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>

#include "NHOCameraParameters.hpp"
#include "NHOImage.hpp"

#include "NHOImageStorageUnit.hpp"
#include "NHOHEMStorageUnit.hpp"

int main(int argc, char *argv[])
{

#ifdef _RASPBIAN
    // NETWORK
    NHOImageStorageUnit* client = new NHOImageStorageUnit("localhost", 51717);
#else
//    NHOImageStorageUnit* imgClient = new NHOImageStorageUnit("localhost", 51717, 51718);
    // address of the server to connect to
    NHOImageStorageUnit* imgClient = new NHOImageStorageUnit("192.168.0.22", 51717, 51718);
    NHOHEMStorageUnit* hemClient = new NHOHEMStorageUnit("192.168.0.22", 51719);
//    NHOImageStorageUnit* client = new NHOImageStorageUnit("192.168.0.22", 51717, 51718);
#endif
    imgClient->setImageSize(400, 600);
    imgClient->setImageFormat(NHOImage::FORMAT_RGB);
    imgClient->initiate();
    hemClient->initiate();
    
    std::cout << "Client running  Main Loop\n";
    unsigned int microseconds = 10000000;
    while(1) {
        usleep(microseconds);
    }    
    imgClient->terminate();
    hemClient->terminate();
    return 0;
}

