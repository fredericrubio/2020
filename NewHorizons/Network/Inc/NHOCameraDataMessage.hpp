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
     * COnstructor
     **/
    NHOCameraDataMessage(const long long   pDate);

    /**
     * Serialize the sensor data in order to be sent.
     **/
    virtual bool serialize(NHOCameraData*);
    
    /**
     * Unserialize the sensor data received.
     **/
    virtual bool unserialize();
    
};
#endif /* NHOCameraDataMessage_hpp */
