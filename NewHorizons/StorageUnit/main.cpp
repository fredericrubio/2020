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
#include "InputParser.hpp"
#include "NHOCameraParameters.hpp"
#include "NHOConfiguration.hpp"
#include "NHOLOG.hpp"

int main(int argc, char **argv)
{

    std::string lConfigurationFileName;
    InputParser input(argc, (char **) argv);
    if(input.cmdOptionExists("-root")){
        const std::string &root = input.getCmdOption("-root");
        lConfigurationFileName = root + std::string("/Resources/rover.xml");
    }
    if (NHOConfiguration::initialize(lConfigurationFileName) == false) {
        NHOFILE_LOG(logERROR) << "Unable to load config file \n";
        return 1;
    };

    NHOCameraParameters* imageServerParameters = NHOConfiguration::getCameraConfiguration();
    NHOSensorParameters* hemParameters = NHOConfiguration::getHEMConfiguration();
#ifdef _RASPBIAN
    // NETWORK
    NHOImageStorageUnit* client = new NHOImageStorageUnit("localhost", 51717);
#else
    // address of the server to connect to
        NHOImageStorageUnit* imgClient = new NHOImageStorageUnit(imageServerParameters->getHostName(), imageServerParameters->getDataEmissionPort(), imageServerParameters->getServiceEmissionPort());
        NHOHEMStorageUnit* hemClient = new NHOHEMStorageUnit("Useless", hemParameters->getDataEmissionPort());
//        NHOImageStorageUnit* imgClient = new NHOImageStorageUnit("192.168.0.22", 51717, 51718);
//        NHOHEMStorageUnit* hemClient = new NHOHEMStorageUnit("192.168.0.22", 51719);
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

