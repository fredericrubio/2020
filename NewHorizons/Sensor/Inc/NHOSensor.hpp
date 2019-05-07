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
#include <mutex>

#include "NHOSensorData.hpp"
#include "NHOEmitter.hpp"
#include "NHOSensorParameters.hpp"

class NHOSensor {
    
public:
    /**
     * Constructor
     **/
    NHOSensor();

    /**
     *
     **/
    ~NHOSensor();
    
    /**
     * Start acquisition
     **/
    virtual bool startAcquisition();
    
    /**
     *
     **/
    virtual bool acquireThread();
        
    /**
     * Stop acquisition
     **/
    virtual bool stopAcquisition();
    
    /**
     * Process acquired data.
     **/
    virtual bool process() = 0;
    
    /**
     * Acquire data from sensor
     **/
    virtual bool acquire() = 0;
    
    /**
     * Send data
     **/
    virtual bool send() = 0;
    
    /**
     * Initiakize sensor
     **/
    virtual bool initialize(const NHOSensorParameters* pParameters);
    
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
    const NHOSensorParameters* parameters;
    /**
     * One channel
     **/
    NHOEmitter* dataEmitter;

    /**
     * Service channnel
     **/
    NHOEmitter* serviceEmitter;

    /// Mutex
    std::mutex mutex;

    
    
};

#endif /* NHOSensor_hpp */
