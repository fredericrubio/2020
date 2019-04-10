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
protected:
    /**
     * Instance Variables
     **/
    unsigned short frequency;
    
public:
    /**
     * Setters/Getters
     **/
    inline void setFrequency(const unsigned short pFrequecy) {
        frequency = pFrequecy;
    };
    
    inline unsigned short gerFrequency() {
        return frequency;
    }

protected:
    /**
     * Constructor
     **/
    NHOSensorParameters();
    
    /**
     * Constructor
     **/
    NHOSensorParameters(const unsigned short pFrequecy);
};
#endif /* NHOSensorParameters_hpp */

