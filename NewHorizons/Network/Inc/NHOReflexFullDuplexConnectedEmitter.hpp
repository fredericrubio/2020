//
//  NHOReflexFullDuplexConnectedEmitter_hpp
//  Network
//
//  Created by Frédéric Rubio on 27/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOReflexFullDuplexConnectedEmitter_hpp
#define NHOReflexFullDuplexConnectedEmitter_hpp

#include <stdio.h>
#include <thread>

#include "NHOFullDuplexConnectedEmitter.hpp"

class NHOReflexFullDuplexConnectedEmitter : public NHOFullDuplexConnectedEmitter {
    
public:
    /**
     * Constructor
     **/
    NHOReflexFullDuplexConnectedEmitter(const unsigned short pPort, const unsigned short pPeriod);

    void setReflexMessage(NHOMessage* pMsg) {message = pMsg;};
    
    bool echoing();

protected:
    
    ////////////////////////////////
    // Never ending loop.
    // Wait and register a connexion on the main socket.
    // This socket is the exit for captured images.
    virtual bool waitForConnectionOnSocket();
    
    NHOMessage* message;
    
};
#endif /* NHOReflexFullDuplexConnectedEmitter_hpp */
