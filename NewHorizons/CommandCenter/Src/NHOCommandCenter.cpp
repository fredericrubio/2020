//
//  NHOCommandCenter.cpp
//  CommandCenter
//
//  Created by Frédéric Rubio on 01/06/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include "NHOCommandCenter.hpp"

//NHOTemplateFullDuplexConnectedEmitter<NHOTCMessage>* NHOCommandCenter::sEmitter = NULL;
//NHOTemplateBroadcastReceiver<NHOHEMMessage>* NHOCommandCenter::sHEMReceiver = NULL;

static NHOCommandCenter* singleton = NULL;

/**
 * Get the singleton.
 **/
NHOCommandCenter* const NHOCommandCenter::get() {
    if (singleton == NULL) {
        singleton = new NHOCommandCenter();
    }
    return singleton;
}

void NHOCommandCenter::refresh(NHOHEMMessage* const parameter) {
    
    if (parameter == NULL) {
        return;
    }
    
    mutex.lock();
    if (this->hem) {
        delete this->hem;
        this->hem = NULL;
    }
    this->hem = new NHOHEMMessage(*parameter);
    mutex.unlock();
    return;
}


/**
 * Constructor
 **/
NHOCommandCenter::NHOCommandCenter() {
    
    this->hem = new NHOHEMMessage(clock());

    // NETWORK
    this->emitter = new NHOTemplateFullDuplexConnectedEmitter<NHOTCMessage>(51720) ;
    this->HEMReceiver = new NHOTemplateBroadcastReceiver<NHOHEMMessage>(51719);
    this->HEMReceiver->attach(this);
}

bool NHOCommandCenter::sendForwardTC() {
    
    if (! NHOCommandCenter::get()->getEmitter()) {
        return false;
    }
    
    // build the TC
    NHOTCMessage   message(clock());
    
    message.setCommand(NHOTCMessage::eForward);
    message.setMagnitude(1.0);
    message.setDuration(-1.0);
    message.setSpeed(-1.0);
    message.serialize();
    
    NHOCommandCenter::get()->getEmitter()->send(&message);
    
    return true;
}

bool  NHOCommandCenter::sendReverseTC() {
    
    if (! NHOCommandCenter::get()->getEmitter()) {
        return false;
    }
    
    // build the TC
    NHOTCMessage   message(clock());
    
    message.setCommand(NHOTCMessage::eReverse);
    message.setMagnitude(2.0);
    message.setDuration(-1.0);
    message.setSpeed(-1.0);
    message.serialize();
    
    NHOCommandCenter::get()->getEmitter()->send(&message);
    
    return true;
}

bool  NHOCommandCenter::sendStopTC() {
    if (! NHOCommandCenter::get()->getEmitter()) {
        return false;
    }
    
    // build the TC
    NHOTCMessage   message(clock());
    
    message.setCommand(NHOTCMessage::eStop);
    message.setMagnitude(-1.0);
    message.setDuration(-1.0);
    message.setSpeed(-1.0);
    message.serialize();
    
    NHOCommandCenter::get()->getEmitter()->send(&message);
    
    return true;
}

bool  NHOCommandCenter::sendTurnLeftTC() {
    if (! NHOCommandCenter::get()->getEmitter()) {
        return false;
    }
    
    // build the TC
    NHOTCMessage   message(clock());
    
    message.setCommand(NHOTCMessage::eTurnLeft);
    message.setMagnitude(-1.0);
    message.setDuration(-1.0);
    message.setSpeed(-1.0);
    message.serialize();
    
    NHOCommandCenter::get()->getEmitter()->send(&message);
    
    return true;
}

bool  NHOCommandCenter::sendTurnRightTC() {
    if (! NHOCommandCenter::get()->getEmitter()) {
        return false;
    }
    
    // build the TC
    NHOTCMessage   message(clock());
    
    message.setCommand(NHOTCMessage::eTurnRight);
    message.setMagnitude(-1.0);
    message.setDuration(-1.0);
    message.setSpeed(10.0);
    message.serialize();
    
    NHOCommandCenter::get()->getEmitter()->send(&message);

    return true;
}

/**
 * Initialize whatever needs to be intialized (netwotk stuff)
 **/
bool NHOCommandCenter::initialize() {
    bool lReturnE = get()->getEmitter()->initiate();
    if (lReturnE == false) {
        get()->getEmitter()->terminate();
    }
    bool lReturnH = get()->getHEMReceiver()->initiate() ;
    if (lReturnH == false) {
        get()->getHEMReceiver()->terminate();
    }
    return lReturnH && lReturnE ;
}

/**
 * Terminate properly and safely
 **/
bool NHOCommandCenter::terminate() {
    if (singleton) {
        singleton->finish();
        delete singleton;
    }
    return true;
}

/**
 * Destructor.
 **/
NHOCommandCenter::~NHOCommandCenter() {
    if (this->hem) {
        delete this->hem;
        this->hem = NULL;
    }
    if (this->emitter) {
        delete this->emitter;
        this->emitter = NULL;
    }
    if (this->HEMReceiver) {
        delete HEMReceiver;
        this->HEMReceiver = NULL;
    }
}

/**
 * Terminate properly and safely
 **/
bool NHOCommandCenter::finish() {
    bool lReturn = emitter->terminate();
    lReturn = HEMReceiver->terminate() && lReturn;
    return lReturn;
}


