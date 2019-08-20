//
//  NHORoverHEM.cpp
//  Sensor
//
//  Created by Frédéric Rubio on 14/05/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include "NHORoverHEM.hpp"

#include "NHOHEMMessage.hpp"
#include "NHOHEMData.hpp"
#include "NHOBroadcastEmitter.hpp"
#include "NHOLOG.hpp"

/**
 *
 **/
NHORoverHEM::NHORoverHEM() {
    
    this->data = new NHOHEMData();

}

/**
 *
 **/
NHORoverHEM::~NHORoverHEM(){
    
}

/**
 * Acquire data from sensor
 **/
bool NHORoverHEM::acquire(){
    
    // store date
    NHOHEMData* lData = dynamic_cast<NHOHEMData*>(this->data);

    // update data
    lData->fetch();

    return true;
    
}

/**
 * Send data
 **/
bool NHORoverHEM::send(){
    //    NHOFILE_LOG(logDEBUG) << "NHORoverHEM::send: " << std::endl;
   
    NHOHEMMessage* lMessage = NULL;
    bool lReturn = false;

    // send data
    // do not forget to thread the emission => mutex management on image data
    if (parameters->isDataEmitterOn()) {
        // instanciate message to send
        lMessage = dynamic_cast<NHOHEMMessage*>(NHOMessageFactory::build(data));
        // send mesg and get status
        lMessage->serialize();
        lReturn = dataEmitter->send(lMessage);
        if (! lReturn) {
            NHOFILE_LOG(logERROR) << "NHORoverHEM::send: failed to send HEM\n";
        }
//        else {
//            NHOFILE_LOG(logDEBUG) << "NHORoverHEM::send: " << std::endl;
//        }
    }
    
    //memory management
    // TO DO a copy constructor in the factory: objective delete the NHOCameraDataMessage independently of the lifecyscle
    // of the field data
    //    if (lMessage) {
    //        delete lMessage;
    //    }
    
    if (lMessage != NULL) {
        delete lMessage;
    }
    
    return lReturn;
}

/**
 * Process acquired data.
 **/
bool NHORoverHEM::process() {
    
    return true;
    
}

/**
 * Initiakize sensor
 **/
bool NHORoverHEM::initialize(const NHOSensorParameters* pParameters) {
    
    // sensor part initializing
    if (!NHOSensor::initialize(pParameters)) {
        return false;
    }

    // data emitter
    dataEmitter = new NHOBroadcastEmitter(pParameters->getDataEmissionPort(),
                                          pParameters->getDataEmissionPeriod());
    if (!dataEmitter->initiate()) {
        return false;
    }

    return true;
    
}
