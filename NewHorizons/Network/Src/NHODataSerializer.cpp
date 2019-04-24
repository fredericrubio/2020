//
//  NHOMessageFactory.cpp
//  Network
//
//  Created by Frédéric Rubio on 22/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include "NHOCameraDataMessage.hpp"
#include "NHOCameraData.hpp"
#include "NHODataSerializer.hpp"

NHODataSerializer::NHODataSerializer() {
    
}

bool NHODataSerializer::serialize(const NHOCameraData* pData) const {
    // NHOMessage(clock());
    
    return true;
    
}
