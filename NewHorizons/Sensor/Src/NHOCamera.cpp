//
//  NHOCamera.cpp
//  Sensor
//
//  Created by Frédéric Rubio on 02/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include "NHOCamera.hpp"

#include "NHOCameraParameters.hpp"
#include "NHOCameraData.hpp"
#include "NHOFullDuplexConnectedEmitter.hpp"
#include "NHOLOG.hpp"
#include "NHOCameraDataMessage.hpp"
#include "NHOMessageFactory.hpp"
#include "NHOImage.hpp"
#include "NHOReflexFullDuplexConnectedEmitter.hpp"
#include "NHOImageSizeMessage.hpp"

NHOCamera::NHOCamera(const unsigned short pCameraId) {

    data = new NHOCameraData(pCameraId);
    ((NHOCameraData*) data)->setImage(new NHOImage());
    parameters = new NHOCameraParameters();
#ifdef _RASPBIAN
    raspCam = new raspicam::RaspiCam();
    raspCam->setFormat(raspicam::RASPICAM_FORMAT_RGB);
#endif
    
}

/**
 *
 **/
NHOCamera::~NHOCamera() {
    
#ifdef _RASPBIAN
    if (raspCam != NULL) {
        delete raspCam;
    }
#endif    
}

/**
 * Process acquired data.
 **/
bool NHOCamera::process() {
    return true;
}

/**
 * Initiakize sensor
 **/
bool NHOCamera::initialize(const NHOCameraParameters* pParameters) {
    
    image = new NHOImage();
    
#ifdef _RASPBIAN
    if (!raspCam) {
        NHOFILE_LOG(logDEBUG) << "NHOCamera::initialize: KO (no rasp camera" << std::endl;
        return false ;
    }
    
    if (raspCam->open()) {
        NHOFILE_LOG(logDEBUG) << "NHOCamera::initialize: OK" << std::endl;

        // set the image attributes
        image->setWidth(raspCam->getWidth());
        image->setHeight(raspCam->getHeight());
        image->setFormat(NHOImage::FORMAT_RGB);
    }
    else {
        // error management
        NHOFILE_LOG(logERROR) << "NHOCamera::initialize: Error opening camera" << std::endl;
        return false;
    }
#else
    std::string lFileName = "/Users/fredericrubio/Development/Project/New Horizons/Development/test.ppm";
    if (!image->readPPM(lFileName.c_str())) {
        NHOFILE_LOG(logERROR) << "NHOCamera::initialize: Error opening reference image" << std::endl;
        return false;
    }
#endif
    
    if (!pParameters) {
        NHOFILE_LOG(logERROR) << "NHOCamera::initialize: OK" << std::endl;
        return false;
    }
    // sensor part initializing
    if (!NHOSensor::initialize(pParameters)) {
        return false;
    }
    
    // data emitter
    dataEmitter = new NHOFullDuplexConnectedEmitter(pParameters->getDataEmissionPort(), pParameters->getDataEmissionPeriod());
    if (!dataEmitter->initiate()) {
        return false;
    }
    
    // service emitter
    serviceEmitter = new NHOReflexFullDuplexConnectedEmitter(pParameters->getServiceEmissionPort());
    NHOImageSizeMessage* lReflexMessage = new NHOImageSizeMessage(clock(),
                                                                  image->getWidth(),
                                                                  image->getHeight(),
                                                                  image->getFormat(),
                                                                  0);
    lReflexMessage->serialize();
    ((NHOReflexFullDuplexConnectedEmitter *) serviceEmitter)->setReflexMessage(lReflexMessage);
    if (!serviceEmitter->initiate()) {
        return false;
    }
    
    return true;
}

/**
 * True means that the sensor is ready to acquire.
 **/
bool NHOCamera::isReady() {
#ifdef _RASPBIAN
    if (raspCam == NULL) {
        NHOFILE_LOG(logDEBUG) << "NHOCamera::isReady: Camera not initialized" << std::endl;
        return false;
    }
    ready = raspCam->isOpened();
#else
    ready = (image != NULL);
#endif
    return ready;
}


/**
 * Send data
 **/
bool NHOCamera::send() {
//    NHOFILE_LOG(logDEBUG) << "NHOCamera::send: " << std::endl;
    
    NHOCameraDataMessage* lMessage  = NULL;
    bool lReturn = false;
    
    // send data
    // do not forget to thread the emission => mutex management on image data
    if (parameters->isDataEmitterOn()) {
        // instanciate message to send
        lMessage = NHOMessageFactory::build(dynamic_cast<NHOCameraData*>(data));
        // send mesg and get status
        lMessage->serialize();
        lReturn = dataEmitter->send(lMessage);
    }
    
    //memory management
    // TO DO a copy constructor in the factory: objective delete the NHOCameraDataMessage independently of the lifecyscle
    // of the field data
    if (lMessage) {
        delete lMessage;
    }
    
    return lReturn;
}

/**
 * Acquire data from senso
 **/
bool NHOCamera::acquire() {
    bool lCapture = false;
#ifdef _RASPBIAN
    lCapture = raspCam->grab();
    
    NHOFILE_LOG(logDEBUG) << "NHOCamera::acquire: " << lCapture << std::endl;
    
    if (! lCapture) {
        return lCapture;
    }
    
    unsigned char* lImageBuffer = NULL;
    unsigned int lSize = 0;
    // get the image
    lSize = raspCam->getImageTypeSize( raspicam::RASPICAM_FORMAT_RGB );
    if (lSize > 0) {
        // allocate
        lImageBuffer = new unsigned char[lSize];
        //extract the image in rgb format
        raspCam->retrieve(lImageBuffer, raspicam::RASPICAM_FORMAT_IGNORE );
        // store date
        NHOCameraData* lData = dynamic_cast<NHOCameraData*>(data);
        lData->getImage()->setWidth(image->getWidth());
        lData->getImage()->setHeight(image->getHeight());
        lData->getImage()->setFormat(NHOImage::FORMAT_RGB);
        lData->getImage()->setPixels(lSize, lImageBuffer);
    }
#else
    // To do
    // Read an image (rgb) from a file
    std::string lFileName = "/Users/fredericrubio/Development/Project/New Horizons/Development/test.ppm";
    dynamic_cast<NHOCameraData*>(data)->getImage()->readPPM(lFileName.c_str());
    
    lCapture = true;
#endif
    NHOFILE_LOG(logDEBUG) << "NHOCamera::acquire. " << std::endl;
    return lCapture;
}

/*
 * Getters
 */
unsigned int NHOCamera::getWidth() const {
#ifdef _RASPBIAN
    if (raspCam == NULL) {
        NHOFILE_LOG(logDEBUG) << "NHOCamera::getWidth: Camera not initialized" << std::endl;
        return false;
    }
    
    return raspCam->getWidth();
#else
    return image->getWidth();
#endif
}

unsigned int NHOCamera::getHeight() const {
#ifdef _RASPBIAN
    if (raspCam == NULL) {
        NHOFILE_LOG(logDEBUG) << "NHOCamera::getHeight: Camera not initialized" << std::endl;
        return false;
    }
    
    return raspCam->getHeight();
#else
    return image->getHeight();
#endif
}
