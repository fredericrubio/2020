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
    unsigned int infoPort;
    unsigned int dataPort;
    
    int infoConnexionSocket;
    int dataConnexionSocket;
    
    int infoClientSocket; // to manage a client connection (information, service message, commands, ...)
    int dataClientSocket; // to manage a client connection
    
    std::thread* serviceMessageSendingThread;
    std::thread* serviceConnectionThread;
    
    std::thread* dataConnectionThread;

    ////////////////////////////////
    // Never ending loop.
    // Wait and register a connexion on the main socket.
    // Launch a thread to send all service messages at once to the new connected client.
    bool waitForConnectionOnServiceSocket();
    
    ////////////////////////////////
    // Never ending loop.
    // Wait and register a connexion on the main socket.
    // This socket is the exit for captured images.
    bool waitForConnectionOnDataSocket();
    
    /**
     * Manage a connection with a client (channel dedicated to parameters (not images).
     * Send all service messages to a client on a dedicated port.
     **/
    void sendServiceMessages(int pClientPort);
};
#endif /* NHOFullDuplexConnectedEmitter_hpp */
