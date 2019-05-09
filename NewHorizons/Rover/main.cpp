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
    
//    std::string lFileName = "/Users/fredericrubio/Development/Project/New Horizons/Development/test.ppm";
//    NHOImage* lImage = new NHOImage();
//    lImage->readPPM(lFileName.c_str());
//    lImage->saveToDisk();
    
    NHOCamera*  lCamera = new NHOCamera(0);
    NHOCameraParameters* pParameters = new NHOCameraParameters();
    
    // insert code here...
    pParameters->setStorage(false);
    pParameters->setDataEmission(true);
    pParameters->setDataEmissionPort(51717);
    pParameters->setSamplingPeriod(10000);
    pParameters->setServiceEmissionPort(51718);

    if (!lCamera->initialize(pParameters)) {
        NHOFILE_LOG(logERROR) << "Main: Fatal error." << std::endl;
        return 0;
    }
    
    lCamera->startAcquisition();
    
    unsigned int microseconds = 10000000;
    while(true) {
        usleep(microseconds);
    }

    return 0;
    
}
