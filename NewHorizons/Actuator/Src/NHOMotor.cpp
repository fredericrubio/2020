//
//  NHOMotor.cpp
//  CommandCenter
//
//  Created by Frédéric Rubio on 10/06/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include "NHOMotor.hpp"

#include "NHOLOG.hpp"

/**
 * Constructor
 **/
NHOMotor::NHOMotor(unsigned short pPort1, unsigned short pPort2, unsigned short pPort3):
port1(pPort1), port2(pPort2), port3(pPort3), speed(0) {
    
}

// -- Commands --
/**
 * Forward
 **/
bool NHOMotor::forward(){
    
#ifdef _RASPBEIN
#else
    NHOFILE_LOG(logDEBUG) << "NHOMotor::forward forwarding." << std::endl;
#endif

    return true;
    
}

/**
 * Reverse
 **/
bool NHOMotor::reverse(){
    
#ifdef _RASPBEIN
#else
    NHOFILE_LOG(logDEBUG) << "NHOMotor::reverse reversing." << std::endl;
#endif
    
    return true;
    
}

/**
 * Break (fast stop)
 **/
bool NHOMotor::stifle() {

#ifdef _RASPBEIN
#else
    NHOFILE_LOG(logDEBUG) << "NHOMotor::stifle." << std::endl;
#endif

    return true;
    
}

/**
 * Free wheel
 **/
bool NHOMotor::freeWheel() {

#ifdef _RASPBEIN
#else
    NHOFILE_LOG(logDEBUG) << "NHOMotor::freeWheel." << std::endl;
#endif

    return true;
    
}

/**
 * Set speed
 **/
bool NHOMotor::setSpeed(unsigned short pSpeed) {
    
    speed = pSpeed;
    
#ifdef _RASPBIAN
#else
#endif
    
    return true;
    
}

