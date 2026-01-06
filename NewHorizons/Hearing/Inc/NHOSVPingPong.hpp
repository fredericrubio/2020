//
//  NHOSVPingPong.hpp
//  Hearing
//
//  Created by Frédéric Rubio on 13/09/2025.
//  Copyright © 2025 Frédéric Rubio. All rights reserved.
//

#ifndef NHOSVPingPong_hpp
#define NHOSVPingPong_hpp

#include <stdio.h>

#include "NHOSolenoidValveMessageDispatcher.hpp"
#include "NHOTemplateBroadcaster.hpp"
#ifdef ESP32_ADAFRUIT_FEATHER
#include "Utils/NHOComponent.hpp"
#include "Utils/NHOMediator.hpp"
#else
#include "NHOComponent.hpp"
#include "NHOMediator.hpp"
#endif

class NHOSVEmRec;

class NHOSVPingPong : public NHOSolenoidValveMessageDispatcher::NHORegistree, public NHOComponent {
    
    public :
//        typedef enum {eUnknown, ePing, ePong, eAckMessage, eNackMessage} NHOSolenoidValvePayload;

    /**
     *
     **/
#ifdef ESP32_ADAFRUIT_FEATHER
    NHOSVPingPong(  const NHOTemplateBroadcaster<NHOSolenoidValveMessage>* pBroadcast, 
                    const String role);
#else
    NHOSVPingPong(  const NHOTemplateBroadcaster<NHOSolenoidValveMessage>* pBroadcast, 
                    const std::string role
                );
#endif    
    /**
     *
     **/
    ~NHOSVPingPong();
    
    /**
     * if msg->load == pong -> stop waiting for an acq
     **/
    virtual bool process(const NHOSolenoidValveMessage* const pMsg);
    
    /**
     * Yet another loop:
     * if elapsed time since last ping > threshold_1
     *      failed attempt =+1
     *      if failed attempt == 2 -> send warning
     * if elpased time since last ping > threshold_2
     *      send ping
     *      set last ping date
     **/
    virtual bool loop();

    /*
     * Convention: lastPingDate == 0 -> waiting for nothing
     */
    inline bool isWaitingForAcknowledgement() const { return this->getLastPingDate() != 0;};
    
    virtual void setLastPingDate(const long long pDate);
    virtual void setFailedAttempts(const unsigned short pNumber);
    
    virtual long long getLastPingDate() const;
    virtual unsigned short getFailedAttempts() const;
    
    /*
     * 
     */
    inline unsigned short getDelayPing() const {return this->delayPing;};
    inline unsigned short getDelayAck() const {return this->delayAck;};

    inline void setDelayPing(unsigned short pValue) {this->delayPing = pValue;};
    inline void setDelayAck(unsigned short pValue) {this->delayAck = pValue;};

    protected :
    const NHOTemplateBroadcaster<NHOSolenoidValveMessage>* broadcast;
    unsigned short  delayPing;
    unsigned short  delayAck;
#ifdef ESP32_ADAFRUIT_FEATHER
    const String    role;
#else
    const std::string    role;
    long long       lastPingDate;
    unsigned short  failedAttempt;
#endif
};
#endif /* NHOSVPingPong_hpp */
