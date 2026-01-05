//
//  NHOProxyAlertCenter.cpp
//  CommandCenter
//
//  Created by Frédéric Rubio on 01/06/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include "NHOProxyAlertCenter.hpp"
#include "NHOConfiguration.hpp"

static NHOProxyAlertCenter* singleton = NULL;

/**
 * Get the singleton.
 **/
NHOProxyAlertCenter* const NHOProxyAlertCenter::get() {
    if (singleton == NULL) {
        singleton = new NHOProxyAlertCenter();
    }
    return singleton;
}

void NHOProxyAlertCenter::refresh(NHOHallEffectMessage* const parameter) {
    
    if (parameter == NULL) {
        return;
    }
    
    mutex.lock();
    if (this->msg) {
        delete this->msg;
        this->msg = NULL;
    }
    this->msg = new NHOHallEffectMessage(*parameter);
    mutex.unlock();
    return;
}


/**
 * Constructor
 **/
NHOProxyAlertCenter::NHOProxyAlertCenter() {
    
    this->msg = new NHOHallEffectMessage(clock());

    // NETWORK
//    NHOSensorParameters* lHEMConfig = NHOConfiguration::getHEMConfiguration();
//    this->msgReceiver = new NHOTemplateBroadcastReceiver<NHOHallEffectMessage>(lHEMConfig->getDataEmissionPort());
    this->msgReceiver = new NHOTemplateBroadcastReceiver<NHOHallEffectMessage>(51719);
    this->msgReceiver->attach(this);
}

/**
 * Initialize whatever needs to be intialized (netwotk stuff)
 **/
bool NHOProxyAlertCenter::initialize() {
    bool lReturnH = get()->getMsgReceiver()->initiate() ;
    if (lReturnH == false) {
        get()->getMsgReceiver()->terminate();
    }
    return lReturnH ;
}

/**
 * Terminate properly and safely
 **/
bool NHOProxyAlertCenter::terminate() {
    if (singleton) {
        singleton->finish();
        delete singleton;
    }
    return true;
}

/**
 * Destructor.
 **/
NHOProxyAlertCenter::~NHOProxyAlertCenter() {
    if (this->msg) {
        delete this->msg;
        this->msg = NULL;
    }
    if (this->msgReceiver) {
        delete msgReceiver;
        this->msgReceiver = NULL;
    }
}

/**
 * Terminate properly and safely
 **/
bool NHOProxyAlertCenter::finish() {
    return msgReceiver->terminate();
}


