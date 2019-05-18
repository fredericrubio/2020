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
#include "NHOImageSizeMessage.hpp"
#include "NHOData.hpp"
#include "NHOHEMMessage.hpp"
#include "NHOHEMData.hpp"

/**
 *
 */
NHOCameraDataMessage* NHOMessageFactory::build(NHOCameraData* pData) {
    NHOCameraDataMessage* lMsg = new NHOCameraDataMessage(clock());
    lMsg->setCameraData(new NHOCameraData(*pData));
    return lMsg;
}

/**
 *
 */
NHOImageSizeMessage* NHOMessageFactory::build(const char* const pData) {
    
    // get the message type
    NHOMessageFactory::NHOMessageType lType = NHOMessage::getType(pData);
    NHOImageSizeMessage* lMessage = NULL;
    switch (lType) {
        case NHOMessageFactory::eImageSize:
            lMessage = new NHOImageSizeMessage(clock());
            lMessage->setData(pData);
            lMessage->unserialize();
        break;
            
        default:
        break;
    }
    
    return lMessage;
}

/**
 *
 */
NHOMessage* NHOMessageFactory::build(const NHOData* const pData) {
    
    // get the message type
    NHOMessageFactory::NHOMessageType lType = pData->getType();
    NHOMessage* lMessage = NULL;
    switch (lType) {
        case NHOMessageFactory::eHEM:
            lMessage = new NHOHEMMessage(clock());
            (dynamic_cast<NHOHEMMessage*> (lMessage))->setHEMData((NHOHEMData*) pData);
            break;
            
        default:
            break;
    }
    
    return lMessage;
}


