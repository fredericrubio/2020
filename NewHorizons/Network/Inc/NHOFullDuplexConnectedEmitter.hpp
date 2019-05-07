//
//  NHOFullDuplexConnectedEmitter.hpp
//  Network
//
//  Created by Frédéric Rubio on 27/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOFullDuplexConnectedEmitter_hpp
#define NHOFullDuplexConnectedEmitter_hpp

#include <stdio.h>
#include <thread>
#include <mutex>

#include "NHOEmitter.hpp"

class NHOFullDuplexConnectedEmitter : public NHOEmitter {
    
public:
    /**
     * Constructor
     **/
    NHOFullDuplexConnectedEmitter(const unsigned short pPort, const unsigned short pPeriod);

    /**
     *
     **/
    bool initiate();
    
    /**
     * Emit one.
     **/
    virtual bool send(const NHOMessage*) const ;

    bool echoing();

protected:
    std::thread* connectionThread;

    ////////////////////////////////
    // Never ending loop.
    // Wait and register a connexion on the main socket.
    // This socket is the exit for captured images.
    bool waitForConnectionOnSocket();
    
    int dataClientSocket;
};
#endif /* NHOFullDuplexConnectedEmitter_hpp */
