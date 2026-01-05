//
//  NHOSVBroadcast.hpp
//  Hearing
//
//  Created by Frédéric Rubio on 14/09/2025.
//  Copyright © 2025 Frédéric Rubio. All rights reserved.
//

#ifndef NHOSVBroadcast_hpp
#define NHOSVBroadcast_hpp

#include <stdio.h>
#include "NHOTemplateBroadcastReceiver.hpp"
#include "NHOSolenoidValveMessage.hpp"
#include "NHOSVEmRec.hpp"

class NHOSVEmRec : public NHOTemplateBroadcastReceiver<NHOSolenoidValveMessage> {
    public :
    /**
     *
     **/
    NHOSVEmRec(int pPort);
    
    /**
     *
     **/
    ~NHOSVEmRec();
    
    /**
     * Emit one.
     **/
    virtual bool send(const  NHOMessage  *) const;

    /**
     * Emit one.
     **/
    virtual bool send(const sockaddr*  address, const  NHOMessage  *) const;
};

#endif /* NHOSVBroadcast_hpp */
