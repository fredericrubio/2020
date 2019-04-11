//
//  NHOCameraData.cpp
//  Sensor
//
//  Created by Frédéric Rubio on 10/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include <iostream>
#include "NHOCameraData.hpp"

/**
 *
 **/
NHOCameraData::NHOCameraData() {
    
}

/**
 *
 **/
const unsigned char* NHOCameraData::getImage(unsigned int* const pSize) {
    return NULL;
}

/**
 *
 **/
void NHOCameraData::setImage(unsigned short pWidth, const unsigned short pHeight, const unsigned char*) {
    
}

/**
 * Serialize the sensor data in order to be sent.
 **/
bool NHOCameraData::serialize() {
    return true;
}

/**
 * Unserialize the sensor data received.
 **/
bool NHOCameraData::unserialize() {
    return true;
}
