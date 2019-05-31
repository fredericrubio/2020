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
     * Constructor
     **/
    NHOSensorParameters();

    /**
     * Setters/Getters
     **/
    virtual inline bool setSamplingPeriod(const unsigned short pPeriod) {
        period = pPeriod;
        return true;
    }
    inline virtual void setDataEmissionPeriod(const unsigned short pPeriod) {
        dataEmissionPeriod = pPeriod;
    };
    inline void setStoragePeriod(const unsigned short pPeriod) {
        storePreriod = pPeriod;
    };
    inline void setDataEmissionPort(const unsigned short pPort) {
        dataEmissionPort = pPort;
    };
    inline void setServiceEmissionPort(const unsigned short pPort) {
        serviceEmissionPort = pPort;
    };
    
    inline void setStorage(const bool pOn) {
        storingOn = pOn;
    };
    inline void setDataEmission(const bool pOn) {
        dataEmittionOn = pOn;
    };
    
    inline const unsigned short gerPeriod() const {
        return period;
    };
    inline const unsigned short getDataEmissionPeriod() const {
        return dataEmissionPeriod;
    };
    inline const unsigned short geStorePeriod() const {
        return storePreriod;
    };
    inline const unsigned short getDataEmissionPort() const {
        return dataEmissionPort;
    };
    inline const unsigned short getServiceEmissionPort() const {
        return serviceEmissionPort;
    };
    
    inline bool isDataEmitterOn() const {
        return dataEmittionOn;
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
    unsigned short dataEmissionPeriod;
    bool dataEmittionOn;
    unsigned short dataEmissionPort;
    
    unsigned short serviceEmissionPort;
    /**
     * Storage configuration parameters
     **/
    // Unit: one in 'period' sample is stored
    unsigned short storePreriod;
    bool storingOn;
    
};
#endif /* NHOSensorParameters_hpp */

