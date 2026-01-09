//
//  NHOSVPingPong.cpp
//  Hearing
//
//  Created by Frédéric Rubio on 13/09/2025.
//  Copyright © 2025 Frédéric Rubio. All rights reserved.
//
#ifdef ESP32_ADAFRUIT_FEATHER
    #include "Utils/config.hpp"
    #include "Utils/NHOLOG.hpp"
    #include "Utils/TS_NTP.hpp"
    #include "Sensor/NHOSolenoidValveData.hpp"
    #include "NHOSVMediator.hpp"
#else
    #include "NHOLOG.hpp"
    #include "TS_NTP.hpp"
#endif
#include "NHOSVPingPong.hpp"

const int NHOSVPingPong::sNbMaxFailures = 10;

/**
 *
 **/
#ifdef ESP32_ADAFRUIT_FEATHER
NHOSVPingPong::NHOSVPingPong(   NHOTemplateBroadcaster<NHOSolenoidValveMessage>* pBroadcast, 
                                const String pRole):
#else
NHOSVPingPong::NHOSVPingPong(   NHOTemplateBroadcaster<NHOSolenoidValveMessage>* pBroadcast, 
                                const std::string pRole):
#endif
delayPing(0), delayAck(0), role(pRole) {
    this->broadcast = pBroadcast;
}

/**
 *
 **/
NHOSVPingPong::~NHOSVPingPong() {
    
}

/**
 *
 **/
bool NHOSVPingPong::process(const NHOSolenoidValveMessage* const pMsg) {
    NHOSolenoidValveMessage* msg = NULL;
    // NHOFILE_LOG(logDEBUG) << "NHOSVPingPong::process : " << ((NHOSolenoidValveData*) pMsg->getSoleniodValveData())->getType() << "\n";
    // NHOFILE_LOG(logDEBUG) << "NHOSVPingPong::process : " << ((NHOSolenoidValveData*) pMsg->getSoleniodValveData())->getCommand() << "\n";
    switch(pMsg->getSoleniodValveData()->getCommand()) {
        case NHOSolenoidValveData::ePing :
            NHOFILE_LOG(logDEBUG) << "NHOSVPingPong::process : ping received." << "\n";
            // ping reception => emission of a 'pong'
            msg =  new NHOSolenoidValveMessage(pMsg);
            msg->getSoleniodValveData()->setCommand(NHOSolenoidValveData::ePong);
            msg->serialize();
#ifdef ESP32_ADAFRUIT_FEATHER
            msg->setAddress(*pMsg->getAddress());
#else
            msg->setAddress(pMsg->getAddress());
#endif
            this->broadcast->send(msg->getAddress(), msg);
            delete msg;

            break;
        case NHOSolenoidValveData::ePong :
//            if msg->load == pong -> stop waiting for an acq
            this->setLastPingDate(0);
            this->setFailedAttempts(0);                
            NHOFILE_LOG(logDEBUG) << "NHOSVPingPong::process : pong received." << "\n";
            break;
        // case NHOSolenoidValveData::eUnknown :
        // case NHOSolenoidValveData::eAckMessage :
        // pas de souscription -> ces messages ne seront jamais traités dans cette méthode
        default :
            ;
    }
    return true;
}

/**
 * Yet another loop:
 * if elapsed time since last ping > threshold_1
 *      failed attempt =+1
 *      if failed attempt == 2 -> send warning
 * if elpased time since last ping > threshold_2
 *      send ping
 *      set last ping date
 **/
bool NHOSVPingPong::loop() {
    
    // NHOFILE_LOG(logDEBUG) << "NHOSVPingPong::process Ping : " << this->getLastPingDate() ;
    // NHOFILE_LOG(logDEBUG) << "NHOSVPingPong::process Attempt : " << this->getFailedAttempts() ;

    long long currentTime = TS_NTP::clockMS();

    if (this->isWaitingForAcknowledgement()) {
        NHOFILE_LOG(logDEBUG) << "NHOSVPingPong::loop : waiting for acknowledgment.";
        if ((currentTime - this->getLastPingDate()) > this->delayAck) {
            NHOFILE_LOG(logDEBUG) << "NHOSVPingPong::loop : " << currentTime - this->getLastPingDate() << "ms since last Pong reception.";
            this->setFailedAttempts(this->getFailedAttempts() + 1);
            NHOFILE_LOG(logDEBUG) << "NHOSVPingPong::loop : number of failures: " << this->getFailedAttempts();
            if (this->getFailedAttempts() >= NHOSVPingPong::sNbMaxFailures) {
                this->setFailedAttempts(0);
                this->setLastPingDate(0);
                (const_cast< NHOTemplateBroadcaster<NHOSolenoidValveMessage>*>(this->broadcast))->send("From " + this->role + ": no acknowledgment to ping."); 
                NHOFILE_LOG(logDEBUG) << "==> NHOSVPingPong::loop sending 'no pong' warning.";
#ifdef ESP32_ADAFRUIT_FEATHER
                // close the solenoid valve
                if (this->mediator != nullptr) {
                    ((NHOSVMediator *) this->mediator)->notify( const_cast<NHOComponent*>(static_cast<NHOComponent*>( this )),
                                                                "close");
                }
#endif
            }
        }
    }
    else if ((currentTime - this->getLastPingDate()) > this->delayPing) {
        NHOFILE_LOG(logDEBUG) << "NHOSVPingPong::process : send first ping.";

        // build ping message
        const NHOSolenoidValveData* data = new NHOSolenoidValveData(NHOSolenoidValveData::ePing, TS_NTP::clockMS());
        NHOSolenoidValveMessage* msg = dynamic_cast<NHOSolenoidValveMessage*>(NHOMessageFactory::build(data));
        if (msg != NULL) {
            msg->computeSize();
            msg->serialize();  

            // send ping message
            ((std::remove_const<NHOTemplateBroadcaster<NHOSolenoidValveMessage>*>::type) (this->broadcast))->
                send(const_cast<NHOSolenoidValveMessage*>( msg ));	
            //  bool result =this->broadcast->send(const_cast< NHOSolenoidValveMessage* >( msg ));	
            // bool  result = broadcast->send(((const NHOSolenoidValveMessage*) msg));
            // set last ping date
            this->setLastPingDate(currentTime);

            delete msg;
        }
    }
    else {
        // kind of reset of the counter
        this->setFailedAttempts(0);
    }
    return true;
}

/*
 *
 */
void NHOSVPingPong::setLastPingDate(const long long pDate) {
#ifdef ESP32_ADAFRUIT_FEATHER
    preferences.putULong64("lastPingDate", pDate);
#else
    this->lastPingDate = pDate;
#endif
}

/*
 *
 */
void NHOSVPingPong::setFailedAttempts(const unsigned short pNumber) {
#ifdef ESP32_ADAFRUIT_FEATHER
    preferences.putUShort("failedAttempt", pNumber);
#else
    this->failedAttempt = pNumber;
#endif
}

/*
 *
 */
long long NHOSVPingPong::getLastPingDate() const {
#ifdef ESP32_ADAFRUIT_FEATHER
    return preferences.getULong64("lastPingDate", 0);
#else
    return this->lastPingDate;
#endif
}

/*
 *
 */
unsigned short NHOSVPingPong::getFailedAttempts() const {
#ifdef ESP32_ADAFRUIT_FEATHER
    return preferences.getUShort("failedAttempt", 0);
#else
    return this->failedAttempt;
#endif
}
