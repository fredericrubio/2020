//
//  NHOCamera.cpp
//  Sensor
//
//  Created by Frédéric Rubio on 02/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include "NHOCamera.hpp"

NHOCamera::NHOCamera() {
    
}

/**
 * Acquire data from sensor
 **/
bool NHOCamera::acquire() {
    return true;
}

/**
 * Process acquired data.
 **/
bool NHOCamera::process() {
    return true;
}


/**
 * Stop acquisition
 **/
bool NHOCamera::stopAcquisition() {
    return true;
}

/**
 * Initiakize sensor
 **/
bool NHOCamera::initialize() {
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
virtual bool NHOCamera::isReady() {
#ifdef _RASPBIAN
    if (raspCam == NULL) {
        NHOCamera(logDEBUG) << "NHOCamera::isReady: Camera not initialized" << std::endl;
        return false;
    }
    return  raspCam->isOpened();
#else
    return ready;
#endif
}

/**
 *
 **/
bool IMP_Camera::captureImage() {
    
    bool lCapture = raspCam->grab();
    
#ifdef _DEBUG
    NHO_FILE_LOG(logDEBUG) << "IMP_Camera::captureImage: " << lCapture << std::endl;
#endif
    
    return lCapture;
    
}

/////////////////////////////////
// Method called by the thread capturging the images.
// Only one image is available: the latest captured.
void IMP_Server::captureImage() {
#ifdef _RASPBIAN
    unsigned char*  lPixels;
    unsigned int    lSize;
    std::chrono::time_point<std::chrono::steady_clock> start_time;
    std::chrono::time_point<std::chrono::steady_clock> end_time;
    unsigned int lElapsedTime = 0;
    
    // check whether the camera is opened or not
    
    do {
        // get time at the begining of the send image process
        start_time = std::chrono::steady_clock::now();
        if (camera.captureImage() == true) {
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
}


