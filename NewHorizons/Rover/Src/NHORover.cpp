//
//  NHORover.cpp
//  Rover
//
//  Created by Frédéric Rubio on 12/05/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include "NHORover.hpp"

#include "NHOLOG.hpp"
#include "NHOCamera.hpp"

/**
 * Constructor
 **/
NHORover::NHORover():
cameraParameters(NULL) {
    
    camera = new NHOCamera(0);
    
}

/**
 * Destructor
 **/
NHORover::~NHORover() {
    
    if (camera) {
        delete camera;
        camera = NULL;
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
    
    return true;
}


/**
 *
 **/
bool NHORover::start() {
    
    return camera->startAcquisition();

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
    
    return true;
    
}
