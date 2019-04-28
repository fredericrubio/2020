//
//  NHOMessage.cpp
//  Network
//
//  Created by Frédéric Rubio on 22/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include <stdlib.h>
#include "NHOMessage.hpp"

/**
 * Constructor
 **/
NHOMessage::NHOMessage(const long long pDate):
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

