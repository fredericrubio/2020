//
//  NHORoverHEM.cpp
//  Sensor
//
//  Created by Frédéric Rubio on 14/05/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include "NHORoverHEM.hpp"

#include "NHOHEMMessage.hpp"
#include "NHOHEMData.hpp"

/**
 *
 **/
NHORoverHEM::NHORoverHEM() {
    
    message = new NHOHEMMessage(clock());
    data = new NHOHEMData();
    message->setHEMData((NHOHEMData*) data);

}

/**
 *
 **/
NHORoverHEM::~NHORoverHEM(){
    
}

/**
 * Acquire data from sensor
 **/
bool NHORoverHEM::acquire(){
    
    return true;
    
}

/**
 * Send data
 **/
bool NHORoverHEM::send(){
    
    return true;
    
}

/**
 * Process acquired data.
 **/
bool NHORoverHEM::process() {
    
    return true;
    
}

/**
 * Initiakize sensor
 **/
bool NHORoverHEM::initialize(const NHOSensorParameters* pParameters) {
    
    return true;
    
}
