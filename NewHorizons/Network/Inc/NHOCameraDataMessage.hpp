//
//  NHOCameraDataMessage.hpp
//  Network
//
//  Created by Frédéric Rubio on 22/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOCameraDataMessage_hpp
#define NHOCameraDataMessage_hpp

#include "NHOMessage.hpp"

class NHOCameraData;

class NHOCameraDataMessage : public NHOMessage{
    
public:
    
    /**
     * Constructor
     **/
    NHOCameraDataMessage(const long long   pDate);

    /**
     * Destructor
     **/
    ~NHOCameraDataMessage();
    
    /**
     * Serialize the sensor data in order to be sent.
     **/
    virtual bool serialize();
    
    /**
     * Unserialize the sensor data received.
     **/
    virtual bool unserialize();
   
    inline void setCameraData(NHOCameraData*  pData) {
        cameraData = pData;
        computeSize();
    };
    
    inline NHOCameraData* const getCameraData() const {return cameraData;};
    
    virtual unsigned int computeSize();

    protected :
    NHOCameraData*  cameraData;
    
};
#endif /* NHOCameraDataMessage_hpp */
