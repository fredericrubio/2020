//
//  NHOSensor.hpp
//  Sensor
//
//  Created by Frédéric Rubio on 30/03/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef Sensor_hpp
#define Sensor_hpp

#include <thread>
#include "NHOSensorData.hpp"
#include "NHOEmitter.hpp"

class NHOSensor {
    
public:
    /**
     * Constructor
     **/
    NHOSensor();

    /**
     *
     **/
    virtual NHOSensor();
    
    /**
     * Start acquisition
     **/
    virtual bool startAcquisition();
    
    /**
     * Stop acquisition
     **/
    virtual bool stopAcquisition() = 0;
    
    /**
     * Process acquired data.
     **/
    virtual bool process() = 0;
    
    /**
     * Acquire data from sensor
     **/
    virtual bool acquire() = 0;
    
    /**
     * Initiakize sensor
     **/
    virtual bool initialize() = 0;
    
    /**
     * True means that the sensor is ready to acquire.
     **/
    virtual inline bool isReady() {return ready;};
    
protected:
    /**
     * True: the sensor is ready to acquire
     **/
    bool ready;
    
    std::thread* acquisitionThread;
    
    NHOSensorData* data;
    
    NHOEmitter* emitter;
    
    /// time elapsed betwween to capture (ms)
    unsigned short period; 
    
};

#endif /* NHOSensor_hpp */
