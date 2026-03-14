//
//  NHOSVCommandSender.cpp
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
    #include "App/NHOSVCommandSender.hpp"
#else
    #include "NHOLOG.hpp"
    #include "TS_NTP.hpp"
    #include "NHOSVCommandSender.hpp"
#endif

const int NHOSVCommandSender::sNbMaxFailures = 10;

/**
 *
 **/
#ifdef ESP32_ADAFRUIT_FEATHER
NHOSVCommandSender::NHOSVCommandSender(   NHOTemplateBroadcaster<NHOSolenoidValveMessage>* pBroadcast,
                                const String pRole):
#else
NHOSVCommandSender::NHOSVCommandSender(   NHOTemplateBroadcaster<NHOSolenoidValveMessage>* pBroadcast,
                                const std::string pRole):
#endif
delayCommand(0), delayAck(0), role(pRole) {
    this->broadcast = pBroadcast;
}

/**
 *
 **/
NHOSVCommandSender::~NHOSVCommandSender() {
    
}

/**
 * Two cases: eAckMessage, eNackMessage
 **/
bool NHOSVCommandSender::process(const NHOSolenoidValveMessage* const pMsg) {
    // NHOFILE_LOG(logDEBUG) << "NHOSVCommandSender::process : " << ((NHOSolenoidValveData*) pMsg->getSoleniodValveData())->getType() << "\n";
    // NHOFILE_LOG(logDEBUG) << "NHOSVCommandSender::process : " << ((NHOSolenoidValveData*) pMsg->getSoleniodValveData())->getCommand() << "\n";
    switch(pMsg->getSoleniodValveData()->getCommand()) {
        case NHOSolenoidValveData::eNackMessage :
            // error managment section
            NHOFILE_LOG(logDEBUG) << "NHOSVCommandSender::process : eNackMessage received." << "\n";
            // send a warning to gardner...
            break;
        case NHOSolenoidValveData::eAckMessage :
            // stop waiting for an acq
            this->setLastCommandDate(0);
            this->setFailedAttempts(0);                
            NHOFILE_LOG(logDEBUG) << "NHOSVCommandSender::process : eAckMessage received." << "\n";
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
 * if elapsed time since last Command > threshold_1
 *      failed attempt =+1
 *      if failed attempt == NHOSVCommandSender::sNbMaxFailures -> send warning
 **/
bool NHOSVCommandSender::loop() {
    
    // NHOFILE_LOG(logDEBUG) << "NHOSVCommandSender::process Ping : " << this->getLastPingDate() ;
    // NHOFILE_LOG(logDEBUG) << "NHOSVCommandSender::process Attempt : " << this->getFailedAttempts() ;

    long long currentTime = TS_NTP::clockMS();

    if (this->isWaitingForAcknowledgement()) {
        NHOFILE_LOG(logDEBUG) << "NHOSVCommandSender::loop : waiting for acknowledgment.";
        if ((currentTime - this->getLastCommandDate()) > this->delayAck) {
            NHOFILE_LOG(logDEBUG) << "NHOSVCommandSender::loop : " << currentTime - this->getLastCommandDate() << "ms since last Pong reception.";
            this->setFailedAttempts(this->getFailedAttempts() + 1);
            NHOFILE_LOG(logDEBUG) << "NHOSVCommandSender::loop : number of failures: " << this->getFailedAttempts();
            if (this->getFailedAttempts() >= NHOSVCommandSender::sNbMaxFailures) {
                this->setFailedAttempts(0);
                this->setLastCommandDate(0);
                (const_cast< NHOTemplateBroadcaster<NHOSolenoidValveMessage>*>(this->broadcast))->send("From " + this->role + ": no acknowledgment to Command.");
                // send a warning to gardner...

                NHOFILE_LOG(logDEBUG) << "==> NHOSVCommandSender::loop sending 'no ackknowledgment' warning.";
                // send another close message
                // to do
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
    else {
        // kind of reset of the counter
        this->setFailedAttempts(0);
    }
    return true;
}

/**
 * send of a command specified by the parameter
 * as a result, wait for an aknowledgment after sending
 * do not send a new command if no ack for the previous one
 **/
bool NHOSVCommandSender::send(const NHOSolenoidValveData::NHOSolenoidValvePayload pPayload) {

    long long currentTime = TS_NTP::clockMS();
    bool result = false;
    
    const NHOSolenoidValveData* data = new NHOSolenoidValveData(pPayload, currentTime);
    NHOSolenoidValveMessage* msg = dynamic_cast<NHOSolenoidValveMessage*>(NHOMessageFactory::build(data));
    if (msg != NULL) {
        msg->computeSize();
        msg->serialize();
        
        // send Command message
        result = ((std::remove_const<NHOTemplateBroadcaster<NHOSolenoidValveMessage>*>::type) (this->broadcast))->
        send(const_cast<NHOSolenoidValveMessage*>( msg ));

        // set last Command date
        this->setLastCommandDate(currentTime);
        
        delete msg;
    }
    delete data;
    return result;
}

/*
 *
 */
void NHOSVCommandSender::setLastCommandDate(const long long pDate) {
#ifdef ESP32_ADAFRUIT_FEATHER
    preferences.putULong64("lastCommandDate", pDate);
#else
    this->lastCommandDate = pDate;
#endif
}

/*
 *
 */
void NHOSVCommandSender::setFailedAttempts(const unsigned short pNumber) {
#ifdef ESP32_ADAFRUIT_FEATHER
    preferences.putUShort("failedCommandAttempt", pNumber);
#else
    this->failedAttempt = pNumber;
#endif
}

/*
 *
 */
long long NHOSVCommandSender::getLastCommandDate() const {
#ifdef ESP32_ADAFRUIT_FEATHER
    return preferences.getULong64("lastCommandDate", 0);
#else
    return this->lastCommandDate;
#endif
}

/*
 *
 */
unsigned short NHOSVCommandSender::getFailedAttempts() const {
#ifdef ESP32_ADAFRUIT_FEATHER
    return preferences.getUShort("failedCommandAttempt", 0);
#else
    return this->failedAttempt;
#endif
}
