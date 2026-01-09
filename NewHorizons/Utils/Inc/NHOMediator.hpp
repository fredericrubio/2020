//
//  NHOMediator.hpp
//  Utils
//
//  Created by Frédéric Rubio on 05/01/2026.
//  Copyright © 2026 Frédéric Rubio. All rights reserved.
//

#ifndef NHOMediator_hpp
#define NHOMediator_hpp

#include <stdio.h>
#include <list>

class NHOComponent;

class NHOMediator {
    
public:
    
#ifdef ESP32_ADAFRUIT_FEATHER
    virtual void notify( NHOComponent* component, String msg) = 0;
#else
    virtual void notify( NHOComponent* component, std::string msg) = 0;
#endif

};

#endif /* NHOMediator_hpp */
