//
//  NHOEmitter.hpp
//  Network
//
//  Created by Frédéric Rubio on 10/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOEmitter_hpp
#define NHOEmitter_hpp

#include "NHOSensorData.hpp"

class NHOEmitter {
    /**
     * Instance variables
     **/
    
public:
    /**
     * Member functions
     **/
    NHOEmitter(const unsigned short pPort, const unsigned short pPeriod);
    
    /**
     * Send the parameter data.
     **/
    virtual bool send(const NHOSensorData* pData);
protected:
    unsigned short port;
    unsigned short sampling;
    unsigned short counter;
    
};
#endif /* NHOEmitter_hpp */
