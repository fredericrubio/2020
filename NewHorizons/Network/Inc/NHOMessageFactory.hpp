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
class NHOImageSizeMessage;
class NHOMessage;
class NHOData;

class NHOMessageFactory {
    
public:
    typedef enum {eUnknown, eAckMessage, eImageSize, eCameraParameters, eImage, eCameraData, eHEM} NHOMessageType;

    /**
     *
     */
    static NHOCameraDataMessage* build(NHOCameraData* pData);
    
    /**
     *
     **/
    static NHOImageSizeMessage* build(const char* const pData);

    /**
     *
     */
    static NHOMessage* build(const NHOData* const pData);
    
};
#endif /* NHOMessageFactory_hpp */
