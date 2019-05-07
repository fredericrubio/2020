//
//  NHOImageSizeMessageBody.cpp
//  ComLib
//
//  Created by Frédéric Rubio on 04/09/2018.
//  Copyright © 2018 Frédéric Rubio. All rights reserved.
//

#include <string.h>
#include <stdlib.h>

#include "NHOImageSizeMessage.hpp"

#include "NHOImage.hpp"

/**
 * Constructors
 **/
NHOImageSizeMessage::NHOImageSizeMessage(long long pDate,
        const unsigned int pWidth, 
        const unsigned int pHeight,
        const unsigned int pSize,
        const unsigned int pFormat):
NHOMessage(pDate, NHOMessageFactory::eImageSize) {
    width = pWidth;
    height = pHeight;
    dataSize = pSize;
    format = pFormat;
    computeSize();
}

NHOImageSizeMessage::NHOImageSizeMessage(long long pDate):
NHOMessage(pDate, NHOMessageFactory::eImageSize) {
    width = 0;
    height = 0;
    dataSize = 0;
    format = NHOImage::FORMAT_IGNORE;
    computeSize();
}

/**
 * Destructor
 **/
NHOImageSizeMessage::~NHOImageSizeMessage() {
    
}

unsigned int NHOImageSizeMessage::computeSize() {
    
    if (size != 0) {
        return size;
    }
    
    size = getHeaderSize();
    size += sizeof(width);
    size += sizeof(height);
    size += sizeof(format);
    return size;
}

bool NHOImageSizeMessage::serialize() {
    
    if (data != NULL) {
        free(data);
    }
    
    // mempry allocation
    data = (char *) calloc(getSize(), sizeof(char));
    
    unsigned int offset = serializeHeader();
    // copy values
    /// width
    memcpy((void *) (data + offset), &width, sizeof(width));
    offset += sizeof(width);
    /// height
    memcpy((void *) (data + offset), &height, sizeof(height));
    offset += sizeof(height);
    /// format
    memcpy((void *) (data + offset), &format, sizeof(format));
    
    return true;
}

bool NHOImageSizeMessage::unserialize() {
  
    if (data == NULL) {
        return false;
    }
    
    // unserialize header
    unsigned int offset = unserializeHeader() ;

    // copy values
    /// width
    memcpy(&width, data  + offset, sizeof(width));
    offset += sizeof(width);
    /// height
    memcpy(&height, data + offset, sizeof(height));
    offset += sizeof(height);
    /// format
    memcpy(&format, data + offset, sizeof(format));
    
    return true;
    
}
