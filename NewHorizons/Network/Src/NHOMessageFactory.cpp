//
//  NHOMessageFactory.cpp
//  Network
//
//  Created by Frédéric Rubio on 24/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include "NHOMessageFactory.hpp"

#include "NHODataSerializer.hpp"
#include "NHOCameraDataMessage.hpp"

static const NHODataSerializer* sSerializer = new NHODataSerializer();

/**
 *
 */
NHOCameraDataMessage* NHOMessageFactory::build(const NHOCameraData* pData) {
    
  //  pData->serialize(serializer);
    
    return NULL;
}
