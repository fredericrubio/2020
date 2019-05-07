//
//  NHOMessage.cpp
//  Network
//
//  Created by Frédéric Rubio on 22/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include <string.h>
#include <stdlib.h>

#include "NHOMessage.hpp"

#include "NHOMessageFactory.hpp"

/**
 * Constructor
 **/
NHOMessage::NHOMessage(const long long pDate,
                       NHOMessageFactory::NHOMessageType pType):
size(0), data(NULL), date(pDate) {
    
}

/**
 * Destructor
 **/
NHOMessage::~NHOMessage() {
    
    if (data) {
        free(data);
    }
    
}

