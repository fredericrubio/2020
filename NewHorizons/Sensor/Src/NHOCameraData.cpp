//
//  NHOCameraData.cpp
//  Sensor
//
//  Created by Frédéric Rubio on 10/04/2019.
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
#include <fstream>
#include <iostream>
#include <sys/ioctl.h>
#include <netinet/tcp.h>

#include <iostream>
#include "NHOCameraData.hpp"

/**
 *
 **/
NHOCameraData::NHOCameraData() {
    
}

/**
 * Save the image to disk (debug purpose)
 **/
bool NHOCameraData::saveToDisk() {
    
     std::string lFileName = std::to_string(clock()) + "_image.ppm";
     std::ofstream outFile ( lFileName ,std::ios::binary );
    outFile<<"P6\n" << "#Raspery\n" << getWidth() << " " << getHeight() << "\n255\n";
     outFile.write ( ( char* ) getPixels(), getDataSize());
     outFile.close();
    
    return true;
}

/**
 * Lad an image from disk (debug purpose)
 **/
bool NHOCameraData::loadFromDisk(const char* pFileName) {
    std::string line;
    std::string lFileName = pFileName;
    std::ifstream inFile ( lFileName ,std::ios::binary );
    std::string delimiter = " ";

    // width height 255\n
    std::getline(inFile,line);
    std::getline(inFile,line);
    std::getline(inFile,line);

    unsigned long lHeightIndex = line.find(delimiter);
    std::string token = line.substr(0, lHeightIndex);
    int lWidth = std::stoi( token );
    
    line.erase(0, lHeightIndex + delimiter.length());
    token = line.substr(0, lHeightIndex);
    int lHeight = std::stoi( token );

    // last useless line
    std::getline(inFile,line);
    
    char* lBuffer = (char *) calloc(lWidth * lHeight * 3, sizeof(char));
    
    inFile >> lBuffer;
    inFile.close();

    return true;

}

/**
 * Serialize the sensor data in order to be sent.
 **/
bool NHOCameraData::serialize(){
    return true;
}
/**
 * Unserialize the sensor data received.
 **/
bool NHOCameraData::unserialize() {
    return true;
}



