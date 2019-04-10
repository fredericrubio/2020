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

public class NHOEmitter {
    /**
     * Instance variables
     **/
    
public:
    /**
     * Member functions
     **/
    NHOEmitter();
    
    /**
     *
     **/
    virtual bool send(const NHOSensorData* pData);
};
#endif /* NHOEmitter_hpp */
