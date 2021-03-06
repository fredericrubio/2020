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
#include <string>

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
    
    inline virtual void setDataEmissionPeriod(const unsigned short pPeriod) {
        dataEmissionPeriod = pPeriod;
        if (dataEmissionPeriod < minimalPeriod) {
            dataEmissionPeriod = minimalPeriod;
        }
    };
    
    inline void setHostName(const std::string&pHostName) {hostname = pHostName;};
    inline std::string getHostName() const {return hostname;};
protected:
    unsigned short  minimalPeriod = 100;
    std::string hostname;
};
#endif /* NHOCameraParameters_hpp */
