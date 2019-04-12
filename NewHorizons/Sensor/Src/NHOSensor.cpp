//
//  Sensor.cpp
//  Sensor
//
//  Created by Frédéric Rubio on 30/03/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include <iostream>

#include "NHOSensor.hpp"
#include "NHOLOG.hpp"

/**
 * Constructor
 **/
NHOSensor::NHOSensor():
ready(false),
acquisitionThread(NULL),
data(NULL),
parameters(NULL),
emitter(NULL) {
    
}

/**
 * Start acquisition
 **/
bool NHOSensor::startAcquisition() {
    
    // check the period
    if (parameters->gerPeriod() == 0) {
         NHOFILE_LOG(logDEBUG)  << "NHOSensor::startAcquisition: period is zero. Thread not started." << std::endl;
        return false;
    }
    
    // launch the acquisition thread
    acquisitionThread = new std::thread(&NHOSensor::acquireThread, std::ref(*this));
    
    return true;
}

/**
 *
 **/
bool NHOSensor::acquireThread() {
    
    // never ending loop
    do {
        // acquire data
        acquire();
        
        // serialize date
        data->serialize();

        // send data
        if (parameters->isEmitterOn()) {
            emitter->send(data);
        }
        
        // sleep for a while
        std::this_thread::sleep_for (std::chrono::milliseconds(parameters->gerPeriod()));
        
    }
    while(1);
    return false;
}

/**
 * Initiakize sensor
 **/
bool NHOSensor::initialize(const NHOSensorParameters* pParameters) {
    
    parameters = pParameters;
    
    emitter = new NHOEmitter(parameters->getEmissioPort(), parameters->gerEmissionPeriod);
    
    return true;
    
}

