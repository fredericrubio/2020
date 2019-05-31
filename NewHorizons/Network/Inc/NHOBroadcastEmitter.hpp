//
//  NHOBroadCastElitter.hpp
//  Network
//
//  Created by Frédéric Rubio on 12/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOBroadCastElitter_hpp
#define NHOBroadCastElitter_hpp

#include "NHOEmitter.hpp"

class NHOBroadcastEmitter : public NHOEmitter{
    
public:
    /**
     * Constructor
     **/
    NHOBroadcastEmitter(const unsigned short pPort, const unsigned short pPeriod);

    bool initiate();
    
    /**
     * Emit one.
     **/
    virtual bool send(const  NHOMessage  *) const;

};
#endif /* NHOBroadCastElitter_hpp */
