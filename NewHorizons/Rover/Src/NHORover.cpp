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

/**
 * Constructor
 **/
NHORover::NHORover():
cameraParameters(NULL) {
    
    camera = new NHOCamera(0);
    hem = new NHORoverHEM();
    
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

}

/**
 *
 **/
bool NHORover::initialize() {
    
    if (! readConfiguration()) {
        NHOFILE_LOG(logERROR) << "NHORover::initialize: configuration failed." << std::endl;
        return false;
    }
    
    if (! camera->initialize(cameraParameters)) {
        NHOFILE_LOG(logERROR) << "NHORover::initialize: initialization failed." << std::endl;
        return false;
    }
    
    if (! hem->initialize(hemParameters)) {
        NHOFILE_LOG(logERROR) << "NHORover::initialize: initialization failed." << std::endl;
        return false;
    }
    
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
    
    cameraParameters = new NHOCameraParameters();
    
    // insert code here...
    cameraParameters->setStorage(false);
    cameraParameters->setDataEmission(true);
    cameraParameters->setDataEmissionPort(51717);
    cameraParameters->setSamplingPeriod(10000);
    cameraParameters->setServiceEmissionPort(51718);
    
    hemParameters = new NHOSensorParameters();
    hemParameters->setStorage(false);
    hemParameters->setDataEmissionPort(51719);
    hemParameters->setDataEmission(true);
    hemParameters->setSamplingPeriod(1000);
    return true;
    
}
