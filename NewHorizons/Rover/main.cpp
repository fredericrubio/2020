//
//  main.cpp
//  Rover
//
//  Created by Frédéric Rubio on 30/03/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include <unistd.h>

#include "NHOLOG.hpp"
#include "wiringPi.h"

#include "NHOCamera.hpp"

#include "NHOCameraData.hpp"

#include "NHORover.hpp"

int main(int argc, const char * argv[]) {
    
    NHOFILE_LOG(logDEBUG) << "main: start" << std::endl;
#ifdef _RASPBIAN
    if(wiringPiSetup() == -1){ //when initialize wiring failed,print messageto screen
            NHOFILE_LOG(logERROR) << "Setup wiringPi failed !" << std::endl;
            return 1; 
    }
    pinMode(0, OUTPUT);//Set the pin mode
    pinMode(1, OUTPUT);//Set the pin mode
    pinMode(3, OUTPUT);//Set the pin mode
#endif
    
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
