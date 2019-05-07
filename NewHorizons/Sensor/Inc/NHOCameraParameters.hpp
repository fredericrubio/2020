//
//  NHOCameraParameters.hpp
//  Sensor
//
//  Created by Frédéric Rubio on 03/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOCameraParameters_hpp
#define NHOCameraParameters_hpp

#include "NHOSensorParameters.hpp"

class NHOCameraParameters : public NHOSensorParameters {
    /**
     * Variables
     **/
    
public:
    /**
     * Constructor
     **/
    NHOCameraParameters();
    
    /**
     * Constructor
     **/
    NHOCameraParameters(const unsigned pFrequency);
    
    /**
     * Set and check that the acquisition period is not too small.
     **/
    virtual inline bool setSamplingPeriod(const unsigned short pPeriod) {
        if (pPeriod < minimalPeriod) {
            return false;
        }
        period = pPeriod;
        return true;
    }
    
    inline void setMinimalPeriod(const unsigned short pValue) {
        minimalPeriod = pValue;
    }
    
    inline void setServiceEmissionPort(const unsigned short pPort)  {
        servicemissionPort = pPort;
    };
    
    inline unsigned short getServiceEmissionPort() const {
        return servicemissionPort;
    };

protected:
    unsigned short  minimalPeriod = 100;
    unsigned short  servicemissionPort;
    
};
#endif /* NHOCameraParameters_hpp */
