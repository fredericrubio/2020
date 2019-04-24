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

class NHOMessage;

class NHOEmitter {
    
public:
    /**
     * Member functions
     **/
    NHOEmitter(const unsigned short pPort, const unsigned short pPeriod);
    
    /**
     * Emit one.
     **/
    virtual bool send(const NHOMessage*) const ;
    
protected:
    unsigned short port;
    unsigned short sampling;
    unsigned short counter;
    int emissionSocket;

    
};
#endif /* NHOEmitter_hpp */
