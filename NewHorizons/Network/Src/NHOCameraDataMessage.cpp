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
}

/**
 * Serialize the sensor data in order to be sent.
 **/
bool NHOCameraDataMessage::serialize(const NHOCameraData* const pData) {
    
    if (pData == NULL) {
        return false;
    }
    
    NHOCameraData* lData = (NHOCameraData*) pData;
    unsigned int width = lData->getWidth();
    unsigned int height = lData->getHeight();

    // calculate message size (bytes)
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
    memcpy(data + offset, &width, sizeof(width));
    offset += sizeof(width);
    /// height
    memcpy(data + offset, &height, sizeof(height));
    offset += sizeof(height);
    // data
    memcpy(data + offset, lData->getPixels(), lData->getDataSize());
    
    return size;
}

/**
 * Unserialize the sensor data received.
 **/
bool NHOCameraDataMessage::unserialize(NHOCameraData* const pData) const {
    
    if (pData == NULL) {
        return false;
    }
    
    long long date;
    unsigned int width, height;
    
    // date
    memcpy(&date, (void *) data, sizeof(date));
    unsigned int offset = sizeof(date);
    // width
    memcpy(&width, (void *) (data + offset), sizeof(width));
    offset += sizeof(width);
    // height
    memcpy(&height, (void *) (data + offset), sizeof(height));
    offset += sizeof(height);
    // pixels
    unsigned int lSize = width * height * 3;
    unsigned char* pixels = (unsigned char *) calloc(size, sizeof(unsigned char));
    memcpy(pixels, (void *) (data + offset), size * sizeof(unsigned char));

    pData->setWidth(width);
    pData->setHeight(height);
    pData->setPixels(lSize * sizeof(unsigned char), pixels);
    pData->setFormat(NHOCameraData::FORMAT_RGB);
    
    return true;
}
