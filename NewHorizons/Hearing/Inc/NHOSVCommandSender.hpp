//
//  NHOSVPingPong.hpp
//  Hearing
//
//  Created by Frédéric Rubio on 13/09/2025.
//  Copyright © 2025 Frédéric Rubio. All rights reserved.
//

#ifndef NHOSVCommmandSender_hpp
#define NHOSVCommmandSender_hpp

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

class NHOSVCommandSender : public NHOSolenoidValveMessageDispatcher::NHORegistree, public NHOComponent {
    
    public :
    static const int sNbMaxFailures;

    /**
     *
     **/
#ifdef ESP32_ADAFRUIT_FEATHER
    NHOSVCommandSender(  NHOTemplateBroadcaster<NHOSolenoidValveMessage>* pBroadcast,
                    const String role);
#else
    NHOSVCommandSender(  NHOTemplateBroadcaster<NHOSolenoidValveMessage>* pBroadcast,
                    const std::string role
                );
#endif    
    /**
     *
     **/
    ~NHOSVCommandSender();
    
    /**
     * send of a command specified by the parameter
     * as a result, wait for an aknowledgment after sending
     * do not send a new command if no ack for the previous one
     **/
    virtual bool send(const NHOSolenoidValveData::NHOSolenoidValvePayload pMsg);
    
    /**
     * if msg->load == Ack -> stop waiting for an acq
     **/
    virtual bool process(const NHOSolenoidValveMessage* const pMsg);
    
    /**
     * Yet another loop:
     * if elapsed time since last command emission > threshold_1
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
    inline bool isWaitingForAcknowledgement() const { return this->getLastCommandDate() != 0;};
    
    virtual void setLastCommandDate(const long long pDate);
    virtual void setFailedAttempts(const unsigned short pNumber);
    
    virtual long long getLastCommandDate() const;
    virtual unsigned short getFailedAttempts() const;
    
    /*
     * 
     */
    inline unsigned short getDelayCommand() const {return this->delayCommand;};
    inline unsigned short getDelayAck() const {return this->delayAck;};

    inline void setDelayCommand(unsigned short pValue) {this->delayCommand = pValue;};
    inline void setDelayAck(unsigned short pValue) {this->delayAck = pValue;};

    protected :
    NHOTemplateBroadcaster<NHOSolenoidValveMessage>* broadcast;
    unsigned short  delayCommand;
    unsigned short  delayAck;
#ifdef ESP32_ADAFRUIT_FEATHER
    const String    role;
#else
    const std::string    role;
    long long       lastCommandDate;
    unsigned short  failedAttempt;
#endif
};
#endif /* NHOSVCommmandSender_hpp */
