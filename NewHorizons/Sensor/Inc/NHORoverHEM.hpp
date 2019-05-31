//
//  NHORoverHEM.hpp
//  Sensor
//
//  Created by Frédéric Rubio on 14/05/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHORoverHEM_hpp
#define NHORoverHEM_hpp

#include "NHOSensor.hpp"

class NHOSensorParameters;
class NHOHEMMessage;

class NHORoverHEM : public NHOSensor {
     
 public:
     NHORoverHEM();
     
     ~NHORoverHEM();

     /**
      * Acquire data from sensor
      **/
     bool acquire();
     
     /**
      * Send data
      **/
     virtual bool send();
     
     /**
      * Process acquired data.
      **/
     bool process();
     
     /**
      * Initiakize sensor
      **/
     bool initialize(const NHOSensorParameters* pParameters);
    
protected:
    
 };

#endif /* NHORoverHEM_hpp */
