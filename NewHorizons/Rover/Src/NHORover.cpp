//
//  NHORover.cpp
//  Rover
//
//  Created by Frédéric Rubio on 12/05/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

//#include "NHORover.hpp"
#include "NHORover.hpp"

#include "NHOLOG.hpp"
#include "NHOCamera.hpp"
#include "NHOSensorParameters.hpp"
#include "NHORoverHEM.hpp"
#include "NHODEXM.hpp"
#include "NHOWiringPi.hpp"
#include "NHOConfiguration.hpp"
#include "NHOCCParameters.hpp"

/**
 * Constructor
 **/
NHORover::NHORover():
cameraParameters(NULL) {
    
    dex = new NHODEXM();
    camera = new NHOCamera(0);
    hem = new NHORoverHEM();
    
    cameraParameters = NULL;
    hemParameters = NULL;
    ccParameters = NULL;
    
}

/**
 * Destructor
 **/
NHORover::~NHORover() {
    
    if (camera) {
        delete camera;
        camera = NULL;
    }
    
    if (hem) {
        delete hem;
        hem = NULL;
    }
    
    if (dex) {
        delete dex;
        dex = NULL;
    }
    
}

/**
 * Observing an TNHOTCMessage emitter.
 **/
void NHORover::refresh(NHOTCMessage* const pTC) {
    
    if (pTC == NULL) {
        NHOFILE_LOG(logERROR) << "NHORover::refresh: nothing to decod." << std::endl;
        return;
    }
    
    // unserialize
    pTC->unserialize();
    NHOFILE_LOG(logDEBUG) << "NHORover::refresh: Command <" <<
    pTC->getCommand() << "> <" << pTC->getMagnitude() << "> <" <<
    pTC->getDuration() << "> <" << pTC->getSpeed() << ">" << std::endl;

    if (! dex->execute(dynamic_cast<NHOTCMessage *>(pTC))) {
        NHOFILE_LOG(logERROR) << "NHORover::refresh: TC execution failed <"<< NHOWiringPi::ERROR_CODE << ">\n";
    }
}

/**
 *
 **/
bool NHORover::initialize() {
    
    if (! readConfiguration()) {
        NHOFILE_LOG(logERROR) << "NHORover::initialize: configuration failed." << std::endl;
        return false;
    }
    
    // DEXM
    dex->configure();
    
    // NETWORK
    receiver = new NHOTemplateFullDuplexConnectedReceiver<NHOTCMessage>(this->ccParameters->getHostName(), this->ccParameters->getPort());
/*
#ifdef _RASPBIAN
    receiver = new NHOTemplateFullDuplexConnectedReceiver<NHOTCMessage>("192.168.0.13", 51720);
#else
    receiver = new NHOTemplateFullDuplexConnectedReceiver<NHOTCMessage>("192.168.0.13", 51720);
#endif
*/
    if (! camera->initialize(cameraParameters)) {
        NHOFILE_LOG(logERROR) << "NHORover::initialize: CAMERA initialization failed." << std::endl;
        return false;
    }
    
    if (! hem->initialize(hemParameters)) {
        NHOFILE_LOG(logERROR) << "NHORover::initialize: HEM initialization failed." << std::endl;
        return false;
    }
    
    if (! receiver->initiate()) {
        NHOFILE_LOG(logERROR) << "NHORover::initialize: COMMAND PORT initialization failed." << std::endl;
        return false;
    }
    receiver->attach(this);
    
    return true;
}


/**
 *
 **/
bool NHORover::start() {
    
    bool lReturn = false;
    
    lReturn = camera->startAcquisition();
    lReturn = hem->startAcquisition() && lReturn;
    
    return lReturn;

}

/**
 *
 **/
bool NHORover::readConfiguration() {
    
    // NETWORK
    cameraParameters = NHOConfiguration::getCameraConfiguration();
    
    hemParameters = NHOConfiguration::getHEMConfiguration();
    
    ccParameters = NHOConfiguration::getCCConfiguration();
    
/*
    cameraParameters = new NHOCameraParameters();    
    cameraParameters->setStorage(false);
    cameraParameters->setDataEmission(true);//true);
    cameraParameters->setDataEmissionPort(51717);
    cameraParameters->setSamplingPeriod(10000);
    cameraParameters->setServiceEmissionPort(51718);   
    hemParameters = new NHOSensorParameters();
    hemParameters->setStorage(false);
    hemParameters->setDataEmissionPort(51719);
    hemParameters->setDataEmission(true);//true);
    hemParameters->setSamplingPeriod(1000);
 */ 
    return true;
    
}
