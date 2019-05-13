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

int main(int argc, char *argv[])
{

#ifdef _RASPBIAN
    NHOImageStorageUnit* client = new NHOImageStorageUnit("localhost", 51717s);
#else
//    NHOImageStorageUnit* client = new NHOImageStorageUnit("localhost", 51717);
    NHOImageStorageUnit* client = new NHOImageStorageUnit("192.168.0.13", 51717, 51718);
//    NHOImageStorageUnit* client = new NHOImageStorageUnit("192.168.0.22", 51717, 51718);
#endif
    client->setImageSize(400, 600);
    client->setImageFormat(NHOImage::FORMAT_RGB);
    client->initiate();
    
    std::cout << "Client running  Main Loop\n";
    unsigned int microseconds = 10000000;
    while(1) {
        usleep(microseconds);
    }    
    client->terminate();
    return 0;
}

