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
#include<stdio.h>
#include<stdlib.h>

#include "NHOCameraData.hpp"

#include "NHOImage.hpp"

#define RGB_COMPONENT_COLOR 255

/**
 *
 **/
NHOCameraData::NHOCameraData(const unsigned short pCameraId):
image(NULL),
cameraId(pCameraId){
    
}

/**
 * Copy constructor
 **/
NHOCameraData::NHOCameraData(const NHOCameraData & pCameraData) {
    
    cameraId = pCameraData.cameraId;
    image = new NHOImage(*pCameraData.image);

}

/**
 Default destructor
 **/
NHOCameraData::~NHOCameraData() {
    
    if (image) {
        delete image;
        image = NULL;
    }
    
}

