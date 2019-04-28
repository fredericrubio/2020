//
//  NHOMessageFactory.cpp
//  Network
//
//  Created by Frédéric Rubio on 24/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include <time.h>

#include "NHOMessageFactory.hpp"
#include "NHOCameraData.hpp"
#include "NHOCameraDataMessage.hpp"

/**
 *
 */
NHOCameraDataMessage* NHOMessageFactory::build(const NHOCameraData* pData) {
    
    NHOCameraDataMessage* lMsg = new NHOCameraDataMessage(clock());
    lMsg->serialize(pData);

    return lMsg;
}
