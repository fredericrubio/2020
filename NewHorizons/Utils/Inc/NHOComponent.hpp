//
//  NHOComponent.hpp
//  Utils
//
//  Created by Frédéric Rubio on 05/01/2026.
//  Copyright © 2026 Frédéric Rubio. All rights reserved.
//

#ifndef NHOComponent_hpp
#define NHOComponent_hpp

#include <stdio.h>

class NHOMediator;

class NHOComponent {
    
public :    
    NHOComponent( NHOMediator* pMediator = nullptr);
    
    inline void setMediator( NHOMediator* pMediator) {
        this->mediator = pMediator;
    }
    
protected :
      NHOMediator* mediator;
    
};

#endif /* NHOComponent_hpp */
