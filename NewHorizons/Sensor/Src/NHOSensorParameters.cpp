//
//  NHOSensorParameters.cpp
//  Sensor
//
//  Created by Frédéric Rubio on 03/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include "NHOSensorParameters.hpp"

/**
 * Constructor
 **/
NHOSensorParameters::NHOSensorParameters() {
    frequency = 0;
}

/**
 * Constructor
 **/
NHOSensorParameters::NHOSensorParameters(const unsigned short pFrequency):
frequency(pFrequency) {
}
