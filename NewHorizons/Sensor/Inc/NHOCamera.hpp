//
//  NHOCamera.hpp
//  Sensor
//
//  Created by Frédéric Rubio on 02/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOCamera_hpp
#define NHOCamera_hpp

#include "NHOSensor.hpp"
#include "NHOCameraParameters.hpp"

#ifdef _RASPBIAN
// Raspberry OS
#include "raspicam.h"
#endif

class NHOCamera : public NHOSensor{
    
    /**
     * Attributes
     **/
    
public:
    /**
     * Constructor
     **/
    NHOCamera();
    
    /**
     *
     **/
    ~NHOCamera();
    
    /**
     * Acquire data from sensor
     **/
    bool acquire();
    
    /**
     * Send data
     **/
    virtual bool send();
    
    /**
     * Process acquired data.
     **/
    bool process();
    
    /**
     * Initiakize sensor
     **/
    bool initialize(const NHOCameraParameters* pParameters);
    
    /**
     * True means that the sensor is ready to acquire.
     **/
    virtual bool isReady();
    
protected:
    #ifdef _RASPBIAN
        raspicam::RaspiCam* raspCam;
    #else
    #endif
    
    unsigned int getWidth() const;
    unsigned int getHeight() const;
};

#endif /* NHOCamera_hpp */
