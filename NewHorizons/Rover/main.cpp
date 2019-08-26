//
//  main.cpp
//  Rover
//
//  Created by Frédéric Rubio on 30/03/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include <unistd.h>

#include "NHOLOG.hpp"
#ifdef _RASPBIAN
    #include "wiringPi.h"
#endif
#include "NHOCamera.hpp"

#include "NHOCameraData.hpp"

#include "NHORover.hpp"
#include "tinyxml2.h"
#include "NHOConfiguration.hpp"
#include "NHOCCParameters.hpp"
#include "InputParser.hpp"

int main(int argc, const char **argv) {
    
    std::string lConfigurationFileName;
    InputParser input(argc, (char **) argv);
    if(input.cmdOptionExists("-root")){
        const std::string &root = input.getCmdOption("-root");
        lConfigurationFileName = root + std::string("/Resources/rover.xml");
    }
    if (NHOConfiguration::initialize(lConfigurationFileName) == false) {
        NHOFILE_LOG(logERROR) << "Unable to load config file \n";
        return 1;
    };

    NHOCameraParameters* lCameraConfig = NHOConfiguration::getCameraConfiguration();
    
    NHOSensorParameters* lHEMConfig = NHOConfiguration::getHEMConfiguration();
    
    NHOCCParameters* lCCParameters = NHOConfiguration::getCCConfiguration();
    
    NHORoverParameters* lRoverParameters = NHOConfiguration::getRoverConfiguration();

    NHOFILE_LOG(logDEBUG) << "main: start" << std::endl;
    NHORover*   lRover;
    lRover = new NHORover();
    if (!lRover->initialize()) {
        NHOFILE_LOG(logERROR) << "Rover initialization failed !" << std::endl;
        return -1;
    }
    
    lRover->start();
    
    unsigned int microseconds = 10000000;
    while(true) {
        usleep(microseconds);
    }

    return 0;
    
}
