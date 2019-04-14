//
//  NHOEmitter.cpp
//  Network
//
//  Created by Frédéric Rubio on 10/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include "NHOEmitter.hpp"
#include "NHOLOG.hpp"

/**
 * Member functions
 **/
NHOEmitter::NHOEmitter(const unsigned short pPort, const unsigned short pPeriod) {
    
    port = pPort;
    sampling = pPeriod;
    counter = 0;
    
}

/**
 *
 **/
bool NHOEmitter::send(const NHOSensorData* pData) {
 
    NHOFILE_LOG(logERROR) << "NHOEmitter::send." << std::endl;

    counter++;
    if (counter >= sampling) {
        counter = 0;
        // send 
    }
    return (true);
    
}
