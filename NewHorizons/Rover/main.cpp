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
    
    std::string lFileName = "/Users/fredericrubio/Development/Project/New Horizons/Development/test.ppm";
    NHOCameraData* LCameraData = new NHOCameraData();
    LCameraData->loadFromDisk(lFileName.c_str());
    
    bool loadFromDisk(const char* pFileName);
    NHOCamera*  lCamera = new NHOCamera();
    NHOCameraParameters* pParameters = new NHOCameraParameters();
    
    // insert code here...
    NHOFILE_LOG(logDEBUG) << "main: start" << std::endl;
    pParameters->setStorage(false);
    pParameters->setEmission(true);
    pParameters->setEmissionPort(12345);
    pParameters->setSamplingPeriod(100);
    pParameters->setEmissionPeriod(1);

    lCamera->initialize(pParameters);
    lCamera->startAcquisition();
    
    unsigned int microseconds = 10000000;
    while(true) {
        usleep(microseconds);
    }

    return 0;
    
}
