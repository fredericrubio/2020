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

bool NHOAckMessage::serialize(char* const pSerializedArray) {
    
    // copy values
    /// size
    memcpy(pSerializedArray, &value, sizeof(unsigned int));
    
    return true;
}

bool NHOAckMessage::unserialize(const char * const pSerializedArray) {
    
    // copy values
    /// size
    memcpy(&value, pSerializedArray, sizeof(unsigned int));
    
    return true;
    
}

unsigned int NHOAckMessage::getSize() {
    
    return sizeof(unsigned int) ;
    
}
