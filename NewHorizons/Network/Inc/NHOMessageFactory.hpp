//
//  NHOMessageFactory.hpp
//  Network
//
//  Created by Frédéric Rubio on 24/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOMessageFactory_hpp
#define NHOMessageFactory_hpp

class NHOCameraData;
class NHOCameraDataMessage;

class NHOMessageFactory {
    
public:
    typedef enum {eUnknown, eAckMessage, eImageSize, eCameraParameters, eImage, eCameraData} NHOMessageType;

    /**
     *
     */
    static NHOCameraDataMessage* build(NHOCameraData* pData);
    
};
#endif /* NHOMessageFactory_hpp */
