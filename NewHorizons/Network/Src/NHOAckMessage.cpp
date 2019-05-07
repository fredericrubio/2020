//
//  NHOAckMessage.cpp
//  Network
//
//  Created by Frédéric Rubio on 28/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include <string.h>
#include <stdlib.h>

#include "NHOAckMessage.hpp"

/**
 * Constructors
 **/
NHOAckMessage::NHOAckMessage(long long pDate):
NHOMessage(pDate, NHOMessageFactory::eAckMessage) {
    value = 0;
    computeSize();
}

NHOAckMessage::NHOAckMessage(long long pDate, const unsigned int pSize):
NHOMessage(pDate, NHOMessageFactory::eAckMessage) {
    value = pSize;
    computeSize();
}

/**
 * Destructor
 **/
NHOAckMessage::~NHOAckMessage() {
    
}

unsigned int NHOAckMessage::computeSize() {
    
    if (size != 0) {
        return size;
    }
    
    size = getHeaderSize();
    size += sizeof(value);
    return size;
    
}

bool NHOAckMessage::serialize() {

    if (data != NULL) {
        free(data);
    }
    
    // mempry allocation
    data = (char *) calloc(getSize(), sizeof(char));
    
    // header
    unsigned int offset = serializeHeader();
    
    /// size
    memcpy((void *) (data + offset), &value, sizeof(value));
    
    return true;
}

bool NHOAckMessage::unserialize() {
    
    // unserialize header
    unsigned int offset = unserializeHeader() ;
    
    // copy values
    /// size
    memcpy(&value, data + offset, sizeof(value));
    
    return true;
    
}
