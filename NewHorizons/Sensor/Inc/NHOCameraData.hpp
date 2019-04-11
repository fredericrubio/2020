//
//  NHOCameraData.hpp
//  Sensor
//
//  Created by Frédéric Rubio on 10/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOCameraData_hpp
#define NHOCameraData_hpp

#include "NHOSensorData.hpp"

class NHOCameraData: public NHOSensorData {
    
protected:
    /**
     *
     **/
    
public:
    /**
     *
     **/
    NHOCameraData();
    
    /**
     *
     **/
    const unsigned char* getImage(unsigned int* const pSize);
    
    /**
     *
     **/
    void setImage(unsigned short pWidth, const unsigned short pHeight, const unsigned char*);
    
    /**
     * Serialize the sensor data in order to be sent.
     **/
    virtual bool serialize();
    
    /**
     * Unserialize the sensor data received.
     **/
    virtual bool unserialize();
    
};
#endif /* NHOCameraData_hpp */
