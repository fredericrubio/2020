//
//  Sensor.cpp
//  Sensor
//
//  Created by Frédéric Rubio on 30/03/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include <iostream>

#include "NHOSensor.hpp"

NHOSensor::NHOSensor():ready(false),
acquisitionThread(NULL),
data(NULL),
parameters(NULL),
emitter(NULL) {
    
}


/**
 * Start acquisition
 **/
bool NHOSensor::startAcquisition() {
    
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
        emitter->send(data);
        
        // sleep for a while
        std::this_thread::sleep_for (std::chrono::milliseconds(period));
        
    }
    while(1);
    return false;
}
