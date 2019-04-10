//
//  NHOCameraParameters.hpp
//  Sensor
//
//  Created by Frédéric Rubio on 03/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOCameraParameters_hpp
#define NHOCameraParameters_hpp

#include "NHOSensorParameters.hpp"

class NHOCameraParameters : public NHOSensorParameters {
    /**
     * Variables
     **/
    
public:
    /**
     * Constructor
     **/
    NHOCameraParameters();
    
    /**
     * Constructor
     **/
    NHOCameraParameters(const unsigned pFrequency);
    
};
#endif /* NHOCameraParameters_hpp */
