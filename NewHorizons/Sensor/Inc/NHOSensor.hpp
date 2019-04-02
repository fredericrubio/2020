//
//  NHOSensor.hpp
//  Sensor
//
//  Created by Frédéric Rubio on 30/03/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef Sensor_hpp
#define Sensor_hpp

#include <stdio.h>

class NHOSensor {
    // instance variables
    
public:
    /**
     * Constructor
     **/
    public NHOSensor();
    
    /**
     * Acquire data from sensor
     **/
    public bool acquire() = 0;
    
    /**
     * Process acquired data.
     **/
    public bool process() = 0;
};

#endif /* NHOSensor_hpp */
