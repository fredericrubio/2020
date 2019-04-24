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
#include "NHOBroadcastEmitter.hpp"
#include "NHOLOG.hpp"
#include "NHOCameraDataMessage.hpp"
#include "NHOMessageFactory.hpp"

NHOCamera::NHOCamera() {

    data = new NHOCameraData();
    parameters = new NHOCameraParameters();
    
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
    
    if (!pParameters) {
        NHOFILE_LOG(logERROR) << "NHOCamera::initialize: OK" << std::endl;
        return false;
    }
    // sensor part initializing
    if (!NHOSensor::initialize(pParameters)) {
        return false;
    }
    
    // emitter
    emitter = new NHOBroadcastEmitter(pParameters->getEmissionPort(), pParameters->gerEmissionPeriod());

#ifdef _RASPBIAN
    if (raspCam->open()) {
        NHOFILE_LOG(logDEBUG) << "NHOCamera::initialize: OK" << std::endl;
        return true;
    }
    // error management
    NHOFILE_LOG(logERROR) << "NHOCamera::initialize: Error opening camera" << std::endl;
    return false;
#else
    return true;
#endif
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
#endif
    return ready;
}


/**
 * Send data
 **/
bool NHOCamera::send() {
    // send data
    // instanciate message to send
    NHOCameraDataMessage* lMessage = NHOMessageFactory::build(dynamic_cast<NHOCameraData*>(data));
    
    // do not forget to thread the emission => mutex management
    if (parameters->isEmitterOn()) {
        return (emitter->send(lMessage));
    }
    return true;
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
    
    unsigned char* lData = NULL;
    unsigned int lSize = 0;
    // get the image
    lSize = raspCam->getImageTypeSize( raspicam::RASPICAM_FORMAT_RGB );
    if (lSize > 0) {
        // allocate
        lData = new unsigned char[*pSize];
        //extract the image in rgb format
        raspCam->retrieve(lData, raspicam::RASPICAM_FORMAT_IGNORE );
        // store date
        data->setImage(unsigned short pWidth, const unsigend short pHeight, lData);
    }
#else
    // To do
    // Read an image (rgb) from a file
    NHOFILE_LOG(logDEBUG) << "NHOCamera::acquire. " << std::endl;
    lCapture = true;
#endif
    return lCapture;
}

/**
 *
 **/
/*bool NHOCamera::acquireThread() {
    /////////////////////////////////
    // Method called by the thread capturging the images.
    // Only one image is available: the latest captured.
#ifdef _RASPBIAN
    unsigned char*  lPixels;
    unsigned int    lSize;
    std::chrono::time_point<std::chrono::steady_clock> start_time;
    std::chrono::time_point<std::chrono::steady_clock> end_time;
    unsigned int lElapsedTime = 0;
    
    do {
        // get time at the begining of the send image process
        start_time = std::chrono::steady_clock::now();
        if (acquire() == true) {
            lPixels = camera.getImage(&lSize);
            if (lSize > 0) {
                // set image pixels
                image->setPixels(lSize, lPixels);
                // send image to client
                sendImage(image);
            }
            // get current time
            end_time = std::chrono::steady_clock::now();
            lElapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
        }
        else {
            // no image to get: elapsed time <- 0
            lElapsedTime = 0;
            NHO_FILE_LOG(logWARNING) << "IMP_Server::captureImage: No Capture" << std::endl;
        }
        
        // if elapsed time is greater than 'period', sleep for another 'period'
        if (lElapsedTime > period) {
            NHO_FILE_LOG(logWARNING) << "IMP_Server::captureImage: elapsed time greater than cycle" << std::endl;
            std::this_thread::sleep_for (std::chrono::milliseconds(period));
        }
        else {
            // reduce the next loop start
            std::this_thread::sleep_for (std::chrono::milliseconds(period - lElapsedTime));
        }
        // memory management
        image->deletePixels();
    }
    while(1);
#endif
    NHOFILE_LOG(logINFO) << "NHOCamera::acquireThread" << std::endl;
    return true;
}
*/
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
    return -1;
#endif
}

unsigned int NHOCamera::getHeight() const {
#ifdef _RASPBIAN
    if (raspCam == NULL) {
        NHO_FILE_LOG(logDEBUG) << "NHOCamera::getHeight: Camera not initialized" << std::endl;
        return false;
    }
    
    return raspCam->getHeight();
#else
    return -1;
#endif
}
