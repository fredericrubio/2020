//
//  NHOTemplateBroadcastReceiver.hpp
//  Network
//
//  Created by Frédéric Rubio on 13/06/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOTemplateBroadcaster_hpp
#define NHOTemplateBroadcaster_hpp

#include <iostream>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sys/ioctl.h>
#include <netinet/tcp.h>
#include <mutex>
#include <thread>

#ifdef ESP32_ADAFRUIT_FEATHER
#include "HTTPClient.h"
#include "UrlEncode.h"
#include "AsyncUDP.h"
#include "Utils/NHOSubject.hpp"
#include "Network/NHOMessage.hpp"
#include "Utils/NHOLOG.hpp"
#include "Utils/TS_NTP.hpp"
#else
#include "NHOSubject.hpp"
#include "NHOMessage.hpp"
#include "NHOLOG.hpp"
#include "TS_NTP.hpp"
#endif


template <class T>
class NHOTemplateBroadcaster: public NHOSubject<T> {
    
public:
    NHOTemplateBroadcaster(const int pConnexionPort) {
        this->port = pConnexionPort;
        this->message = new T(TS_NTP::clockMS());
        this->keepGoing = true;
        this->observers.clear();
    }
    
    /**
     * Initialize network stuff.
     **/
    bool initiate();
    
    /**
     * Terminte reception loop and close socket.
     **/
    bool terminate();
    
    /**
     * Receive
     **/
#ifdef ESP32_ADAFRUIT_FEATHER
    bool receive(AsyncUDPPacket pPacket);
#else
        bool receive();
#endif
            
    /**
     * Emit one.
     **/
#ifdef ESP32_ADAFRUIT_FEATHER
    virtual bool send(const IPAddress*  pAddress, const  NHOMessage  *pMsg) const;
#else
    virtual bool send(const sockaddr*  address, const  NHOMessage  *) const;
#endif

    /**
     * Emit one.
     **/
    virtual bool send(const NHOMessage *  pMsg) ;

    inline unsigned int getEmissionPort() const {return this->port;}; 

    /*
    * Send a message thanks to http api
    */    
#ifdef ESP32_ADAFRUIT_FEATHER
    void send(const String message) const;
#else
    void send(const std::string message) const;
#endif

protected:
    unsigned int port;
#ifdef ESP32_ADAFRUIT_FEATHER
    AsyncUDP udp;
    std::mutex mutex;
#else
    int emrecSocket;
    std::thread* thread;
    std::mutex mutex;
#endif
    T* message;
    bool keepGoing;

};

#include "NHOTemplateBroadcaster_impl.hpp"
#endif /* NHOTemplateBroadcaster_hpp.hpp */
