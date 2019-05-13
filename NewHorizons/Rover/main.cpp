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

#include "NHORover.hpp"

int main(int argc, const char * argv[]) {
    
    NHOFILE_LOG(logDEBUG) << "main: start" << std::endl;
    
    NHORover*   lRover;
    lRover = new NHORover();
    lRover->initialize();
    
    lRover->start();
    
    unsigned int microseconds = 10000000;
    while(true) {
        usleep(microseconds);
    }

    return 0;
    
}
