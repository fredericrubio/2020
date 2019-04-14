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
protected:
    unsigned short   minimalPeriod = 100;
};
#endif /* NHOCameraParameters_hpp */
