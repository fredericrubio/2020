//
//  NHOSensorParameters.hpp
//  Sensor
//
//  Created by Frédéric Rubio on 03/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOSensorParameters_hpp
#define NHOSensorParameters_hpp

class NHOSensorParameters {

public:
    /**
     * Setters/Getters
     **/
    inline void setSamplingReiod(const unsigned short pPeriod) {
        period = pPeriod;
    }
    inline void setEmissionPeriod(const unsigned short pPeriod) {
        emissionPreriod = pPeriod;
    };
    inline void setStoragePeriod(const unsigned short pPeriod) {
        strorePreriod = pPeriod;
    };
    inline void setEmissionPort(const unsigned short pPort) {
        emissionPort = pPort;
    };
    
    inline void setStorage(const bool pOn) {
        emittingOn = pOn;
    };
    inline void setEmission(const bool pOn) {
        storingOn = pOn;
    };
    
    inline unsigned short gerPeriod() const {
        return period;
    };
    inline unsigned short gerEmissionPeriod() const {
        return emissionPreriod;
    };
    inline unsigned short storePeriod() const {
        return strorePreriod;
    };
    inline unsigned short getEmissionPort() const {
        return emissionPort;
    };
    
    inline bool isEmitterOn() const {
        return emittingOn;
    };
    inline bool isStoringOn() const {
        return storingOn;
    };
protected:
    /// time elapsed betwween to capture (ms)
    unsigned short period;
    
    /**
     * Emitter configuration parameters
     **/
    // Unit: one in 'period' sample is emitted
    unsigned short emissionPreriod;
    bool emittingOn;
    unsigned short emissionPort;
    
    /**
     * Storage configuration parameters
     **/
    // Unit: one in 'period' sample is stored
    unsigned short strorePreriod;
    bool storingOn;
    
    /**
     * Constructor
     **/
    NHOSensorParameters();
};
#endif /* NHOSensorParameters_hpp */

