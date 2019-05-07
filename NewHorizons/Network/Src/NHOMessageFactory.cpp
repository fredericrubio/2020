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
NHOCameraDataMessage* NHOMessageFactory::build(NHOCameraData* pData) {
    /*
     switch(type) {
     case eImageSize:
     body = new NHOImageSizeMessageBody();
     break;
     case eCameraParameters:
     case eImage:
     case eUnknown:
     break;
     }
     
*/
    NHOCameraDataMessage* lMsg = new NHOCameraDataMessage(clock());
    lMsg->setCameraData(pData);
    lMsg->serialize();

    return lMsg;
}
