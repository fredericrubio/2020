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
emitter(NULL){
    
}

/**
 * Constructor
 **/
NHOSensor::~NHOSensor() {
    
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
    
    NHOFILE_LOG(logDEBUG)  << "NHOSensor::startAcquisition." << std::endl;
    
    return true;
}

/**
 * Stop acquisition
 **/
bool NHOSensor::stopAcquisition() {
    return true;
}

/**
 *
 **/
bool NHOSensor::acquireThread() {
    
    std::chrono::time_point<std::chrono::steady_clock> start_time;
    std::chrono::time_point<std::chrono::steady_clock> end_time;
    long long lElapsedTime = 0;
    // never ending loop
    do {
        start_time = std::chrono::steady_clock::now();

        // acquire data
        if (acquire()) {

            //
            if (parameters->isEmitterOn()) {
                send();
            }
            
            end_time = std::chrono::steady_clock::now();
            lElapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
        }
        else {
            // no image to get: elapsed time <- 0
            lElapsedTime = 0;
            NHOFILE_LOG(logWARNING) << "NHOSensor::acquireThread: No acquisition" << std::endl;
        }

        // if elapsed time is greater than 'period', sleep for another 'period'
        if (lElapsedTime > parameters->gerPeriod()) {
            NHOFILE_LOG(logWARNING) << "NHOSensor::acquireThread: elapsed time greater than cycle." << std::endl;
            std::this_thread::sleep_for (std::chrono::milliseconds(parameters->gerPeriod()));
        }
        else {
            // reduce the next loop start
            std::this_thread::sleep_for (std::chrono::milliseconds(parameters->gerPeriod() - lElapsedTime));
        }
        
    }
    while(1);
    return false;
}

/**
 * Initiakize sensor
 **/
bool NHOSensor::initialize(const NHOSensorParameters* pParameters) {
    
    parameters = pParameters;
    
    emitter = new NHOEmitter(parameters->getEmissionPort(), parameters->gerEmissionPeriod());
    
    return true;
    
}

