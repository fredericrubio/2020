//
//  NHOMessage.cpp
//  ComLib
//
//  Created by Frédéric Rubio on 04/09/2018.
//  Copyright © 2018 Frédéric Rubio. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "NHOMessage.hpp"
#include "NHOMessageFactory.hpp"

NHOMessage::NHOMessage(long long pDate, NHOMessageFactory::NHOMessageType pType):
date(pDate), type(pType), size(0), data(NULL) {

}

/**
 * Destructor
 **/
NHOMessage::~NHOMessage() {
    
    if (data != NULL) {
        free(data);
    }
    
}

const unsigned int NHOMessage::serializeHeader() {
    
    if (data == NULL) {
        return 0;
    }
    
    // copy values
    unsigned int offset = 0;
    /// date
    memcpy((void *) data, &date, sizeof(date));
    offset += sizeof(date);
    /// type
    memcpy((void *) (data + offset), &type, sizeof(type));
    offset += sizeof(type);
    
    return offset;
}

const unsigned int NHOMessage::unserializeHeader() {
    // calculate message size
    unsigned int offset = 0;
    
    // copy values
    /// date
    memcpy(&date, (void *) data, sizeof(date));
    offset += sizeof(date);
    /// type
    memcpy(&type, data + offset, sizeof(type));
    offset += sizeof(type);
    
    return offset;
}


const unsigned short NHOMessage::getHeaderSize() {
    
    size = sizeof(date);
    size += sizeof(type);

    return size;
}




