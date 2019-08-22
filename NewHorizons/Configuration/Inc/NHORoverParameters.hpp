//
//  NHORoverParameters.hpp
//  Configuration
//
//  Created by Frédéric Rubio on 18/06/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHORoverParameters_hpp
#define NHORoverParameters_hpp

#include "NHOMotorParameters.hpp"

#include <list>

class NHORoverParameters {
    
public:
    /**
     * Constructor
     **/
    NHORoverParameters();
    
    /**
     *
     **/
    inline void addMotor(const NHOMotorParameters* const pMotorParam) {
        motors.push_back((NHOMotorParameters*) pMotorParam);
    }
protected:
    std::list<NHOMotorParameters* > motors;
    
};
#endif /* NHORoverParameters_hpp */
