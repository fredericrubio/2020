//
//  NHOCameraDataMessage.cpp
//  Network
//
//  Created by Frédéric Rubio on 22/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include <stdlib.h>
#include <string.h>

#include "NHOCameraData.hpp"

#include "NHOCameraDataMessage.hpp"

/**
 * COnstructor
 **/
NHOCameraDataMessage::NHOCameraDataMessage(const long long   pDate):
NHOMessage(pDate) {
    
    ;
    
}

/**
 * Serialize the sensor data in order to be sent.
 **/
bool NHOCameraDataMessage::serialize(NHOCameraData* pData) {
    
    if (pData == NULL) {
        return false;
    }
    
    NHOCameraData* lData = (NHOCameraData*) pData;
    unsigned int width = lData->getWidth();
    unsigned int height = lData->getHeight();

    // calculate message size
    size = sizeof(date) + sizeof(width) + sizeof(height) + lData->getDataSize();
    
    // memory management
    if (data != NULL) {
        delete data;
    }
    // memory allocation
    data = (unsigned char *) calloc(size, sizeof(unsigned char));
    
    // copy values
    unsigned int offset = 0;
    /// date
    memcpy((void *) data, &date, sizeof(date));
    offset += sizeof(date);
    /// width
    memcpy(data + sizeof(date), &width, sizeof(width));
    /// height
    memcpy(data + sizeof(width), &height, sizeof(height));
    // data
    memcpy(data + sizeof(height), lData->getPixels(), sizeof(lData->getDataSize()));
    
    return size;
}

/**
 * Unserialize the sensor data received.
 **/
bool NHOCameraDataMessage::unserialize() {
    return true;
}
