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
    
    virtual void notify( NHOComponent* component) = 0;

};

#endif /* NHOMediator_hpp */
