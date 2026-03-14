//
//  NHOMessageDispatcher.cpp
//  Hearing
//
//  Created by Frédéric Rubio on 11/09/2025.
//  Copyright © 2025 Frédéric Rubio. All rights reserved.
//
#ifndef ESP32_ADAFRUIT_FEATHER
    #include "NHOLOG.hpp"
#else
    #include "Utils/NHOLOG.hpp"
#endif

#include "NHOSolenoidValveMessageDispatcher.hpp"

static NHOSolenoidValveMessageDispatcher* singleton = NULL;

/**
 * Constructor
 **/
NHOSolenoidValveMessageDispatcher::NHOSolenoidValveMessageDispatcher() {
    
}

/**
 * Destructor
 **/
NHOSolenoidValveMessageDispatcher::~NHOSolenoidValveMessageDispatcher() {
    
}

/**
 *
 **/
const NHOSolenoidValveMessageDispatcher* NHOSolenoidValveMessageDispatcher::get() {
    
    if (singleton == NULL) {
        singleton = new NHOSolenoidValveMessageDispatcher();
    }
    
    return singleton;
    
}


/**
 *
 **/
//bool NHOMessageDispatcher::enroll(const NHOMessageFactory::NHOMessageType pMsgType,
//                                  const NHORegistree* const pRegistree) {
bool NHOSolenoidValveMessageDispatcher::subscribe(const NHOSolenoidValveData::NHOSolenoidValvePayload pPayload,
                                  const NHORegistree* const pRegistree) {
    
    if (singleton == NULL) {
        NHOFILE_LOG(logDEBUG) << "NHOSolenoidValveMessageDispatcher::enroll singleton pas intancié \n";
        return false;
    }

    if (pRegistree == NULL) {
        NHOFILE_LOG(logDEBUG) << "NHOSolenoidValveMessageDispatcher::enroll : registrre is null\n";
        return false;
    }

    NHORegistreePayload* item = new NHORegistreePayload();
    item->payload = pPayload;
    item->registree = (NHORegistree*) pRegistree;
    
    singleton->registrees.push_back(item);
    
    return true;
}

/**
 *
 **/
bool NHOSolenoidValveMessageDispatcher::dispatch(const NHOSolenoidValveMessage* const pMsg) {
    
    if (singleton == NULL) {
        NHOFILE_LOG(logDEBUG) << "NHOSolenoidValveMessageDispatcher::dispatch singleton pas intancié \n";
        return false;
    }
    
    //  NHOFILE_LOG(logDEBUG) << "NHOSubject::notify <"<< observers.size()<<">\n";
    singleton->mutex.lock();
    // NHOFILE_LOG(logDEBUG) << "NHOSolenoidValveMessageDispatcher::dispatch received " << ((NHOSolenoidValveData*) pMsg->getSoleniodValveData())->getCommand() <<  "\n";
    for (NHORegistreePayload* registree : singleton->registrees) {
        if (registree->payload == ((NHOSolenoidValveData*) pMsg->getSoleniodValveData())->getCommand()) {
            // NHOFILE_LOG(logDEBUG) << "NHOSolenoidValveMessageDispatcher::dispatch dispatching " << registree->payload <<  "\n";
            registree->registree->process(pMsg);
        }
    }
    singleton->mutex.unlock();
    
    return true;
    
}

/**
 *
 **/
void NHOSolenoidValveMessageDispatcher::refresh(NHOSolenoidValveMessage* const pMsg) {
    
    if (pMsg == NULL) {
        return;
    }

    NHOSolenoidValveMessageDispatcher::dispatch(pMsg);
}

/**
 *
 **/
NHOSolenoidValveMessageDispatcher::NHORegistree::NHORegistree() {
    
}

/**
 *
 **/
NHOSolenoidValveMessageDispatcher::NHORegistree::~NHORegistree() {
    
}
