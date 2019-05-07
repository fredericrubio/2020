//
//  main.cpp
//  Rover
//
//  Created by Frédéric Rubio on 30/03/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include <unistd.h>

#include "NHOLOG.hpp"

#include "NHOCamera.hpp"

#include "NHOCameraData.hpp"

int main(int argc, const char * argv[]) {
    
    NHOFILE_LOG(logDEBUG) << "main: start" << std::endl;
    
    std::string lFileName = "/Users/fredericrubio/Development/Project/New Horizons/Development/test.ppm";
//    NHOCameraData* lCameraData = new NHOCameraData();
//    lCameraData->loadFromDisk(lFileName.c_str());
//    lCameraData->readPPM(lFileName.c_str());
//    lCameraData->saveToDisk();
    
    NHOCamera*  lCamera = new NHOCamera();
    NHOCameraParameters* pParameters = new NHOCameraParameters();
    
    // insert code here...
    pParameters->setStorage(false);
    pParameters->setDataEmission(true);
    pParameters->setDataEmissionPort(51717);
    pParameters->setSamplingPeriod(100);
    pParameters->setDataEmissionPeriod(10);

    lCamera->initialize(pParameters);
    lCamera->startAcquisition();
    
    unsigned int microseconds = 10000000;
    while(true) {
        usleep(microseconds);
    }

    return 0;
    
}
