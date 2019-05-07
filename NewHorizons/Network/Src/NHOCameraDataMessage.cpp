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
#include "NHOImage.hpp"

#include "NHOCameraDataMessage.hpp"

/**
 * Constructor
 **/
NHOCameraDataMessage::NHOCameraDataMessage(const long long   pDate):
NHOMessage(pDate, NHOMessageFactory::eCameraData) {

    cameraData = NULL;
    
}


/**
 * Destructor
 **/
NHOCameraDataMessage::~NHOCameraDataMessage() {
    
}

/**
 *
 **/
unsigned int NHOCameraDataMessage::computeSize() {
    
    if (size != 0) {
        return size;
    }
    
    if (cameraData == NULL) {
        return -1;
    }
    
    size = getHeaderSize();
    size += sizeof(cameraData->getId());
    size += sizeof(cameraData->getImage()->getWidth());
    size += sizeof(cameraData->getImage()->getHeight());
    size += sizeof(cameraData->getImage()->getFormat());
    size += cameraData->getImage()->getDataSize();
    
    return size;
}

/**
 * Serialize the sensor data in order to be sent.
 **/
bool NHOCameraDataMessage::serialize() {
    
    if (cameraData == NULL) {
        return false;
    }
    
    unsigned int value;
    
    // mempry allocation
    data = (char *) calloc(getSize(), sizeof(char));
    
    unsigned int offset = serializeHeader();
    // copy values
    /// camera id
    unsigned short cameraId = cameraData->getId();
    memcpy((void *) (data + offset), &cameraId, sizeof(cameraId));
    offset += sizeof(cameraId);
    /// width
    value = cameraData->getImage()->getWidth();
    memcpy((void *) (data + offset), &value, sizeof(value));
    offset += sizeof(value);
    /// height
    value = cameraData->getImage()->getHeight();
    memcpy((void *) (data + offset), &value, sizeof(value));
    offset += sizeof(value);
    /// format
    NHOImage::IMAGE_FORMAT format = cameraData->getImage()->getFormat();
    memcpy((void *) (data + offset), &format, sizeof(format));
    offset += sizeof(format);
    // pixels
    const unsigned char* pixels = cameraData->getImage()->getPixels();
    memcpy((void *) (data + offset), pixels, sizeof(cameraData->getImage()->getDataSize()));
    
    return true;
}

/**
 * Unserialize the sensor data received.
 **/
bool NHOCameraDataMessage::unserialize() {

    if (data == NULL) {
        return false;
    }
    
    // unserialize header
    unsigned int offset = unserializeHeader() ;

    if (cameraData != NULL) {
        delete cameraData;
    }
    unsigned short cameraId = -1;
    unsigned int width, height;
    NHOImage::IMAGE_FORMAT format = NHOImage::FORMAT_IGNORE;
    
    // camera id
    memcpy(&cameraId, (void *) (data + offset), sizeof(cameraId));
    offset += sizeof(width);
    // width
    memcpy(&width, (void *) (data + offset), sizeof(width));
    offset += sizeof(width);
    // height
    memcpy(&height, (void *) (data + offset), sizeof(height));
    offset += sizeof(height);
    // pixels
    unsigned int lSize = width * height * 3;
    unsigned char* pixels = (unsigned char *) calloc(lSize, sizeof(unsigned char));
    memcpy(pixels, (void *) (data + offset), size * sizeof(unsigned char));

    cameraData = new NHOCameraData(cameraId);
    NHOImage* image = new NHOImage();
    image->setWidth(width);
    image->setHeight(height);
    image->setFormat(format);
    image->setPixels(lSize * sizeof(unsigned char), pixels);
    cameraData->setImage(image);
    
    computeSize();
    
    return true;
}
