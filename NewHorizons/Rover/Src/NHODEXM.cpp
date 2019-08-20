//
//  NHODEXM.cpp
//  Rover
//
//  Created by Frédéric Rubio on 03/06/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include "NHODEXM.hpp"

#include <unistd.h>

#include "NHOWiringPi.hpp"
#include "NHOTimer.hpp"
#include "NHOLOG.hpp"

/**
 * Constructor
 **/
NHODEXM::NHODEXM():
motorL(NULL), motorR(NULL),TC(NULL),timer(NULL), motion(false) {
    
    timer = new NHOTimer();
    timer->attach(this);
    
    NHOWiringPi::wiringPiSetup();
}

/**
 * Destructor
 **/
NHODEXM::~NHODEXM() {
    
    if (timer) {
        timer->stop();
        delete timer;
        timer = NULL;
    }
    
}

/**
 * Time is up.
 **/
void NHODEXM::refresh(unsigned int* const parameter) {
    
    NHOFILE_LOG(logDEBUG) << "NHODEXM::timer time is up." << std::endl;
    
    stop();

}


/**
 * Configure.
 **/
bool NHODEXM::configure() {
    
#ifdef _RASPBIAN
    
#else
    motorR = new NHOMotor("Right", 1, 2, 3);
    motorL = new NHOMotor("Left", 4, 5, 6);
#endif
    
    return true;
}


/**
 * WHen time is up, stop any TC in progress
 **/
bool NHODEXM::startTimer(const int pMilliSecs) {
    
    timer->start(pMilliSecs);

    return true;
    
}

/**
 * Consedring the current context, validate (or not) the
 * parameter TC.
 * An obstacle blocking the rover could be a raeson to not validate a TC.
 * A TC must be
 **/
bool NHODEXM::validate()  {
    
    NHOFILE_LOG(logDEBUG) << "NHODEXM::validate." << std::endl;
    
    return true;
    
}

/**
 * Execute the current TC.
 **/
bool NHODEXM::execute() {
    
    NHOFILE_LOG(logDEBUG) << "NHODEXM::execute." << std::endl;
    
    switch(TC->getCommand()) {
        case NHOTCMessage::eForward:
            return forward();
        break;
        case NHOTCMessage::eReverse:
            return reverse();
        break;
        case NHOTCMessage::eStop:
            return stopMotion();
        break;
        case NHOTCMessage::eTurnLeft:
            return turnLeft();
        break;
        case NHOTCMessage::eTurnRight:
            return turnRight();
        break;
        default:
            NHOWiringPi::ERROR_CODE = NHOWiringPi::UNKNOWN_TC;
            return false;
        break;
    }

    return true;
    
}

/**
 * Stop anything in progress.
 **/
bool NHODEXM::stop() {
    
    NHOFILE_LOG(logDEBUG) << "NHODEXM::stop stopped." << std::endl;
    
    timer->stop();
    
    return stopMotion();
    
}

/**
 * Execute the current TC.
 **/
bool NHODEXM::execute( NHOTCMessage*  pTC) {
    
    NHOFILE_LOG(logDEBUG) << "NHODEXM::execute execute." << std::endl;
    
    if (pTC == NULL) {
        NHOWiringPi::ERROR_CODE = NHOWiringPi::INCORRECT_TC;
        return false;
    }
    
    // store TC
    if (TC) {
        delete TC;
    }
    TC = pTC;
    
    bool lStatus = true;
    if (executionInProgress()) {
        lStatus = stop();
    }
    if (!lStatus){
        return false;
    }
    
    return execute();
    
}

/**
 * Return true if a TC execution is in progress.
 **/
bool NHODEXM::executionInProgress() {
    
    NHOFILE_LOG(logDEBUG) << "NHODEXM::executionInProgres." << std::endl;
    if (motion) {
        return true;
    }
    
    if (! timer) {
        return false;
    }
    
    return timer->isTicking();
    
}

// -- COMMAND SECTIONS --
bool NHODEXM::checkMotors() {
    
    if (! motorR) {
        NHOWiringPi::ERROR_CODE = NHOWiringPi::MOTOR1_MISSING;
        return false;
    }
    if (! motorL) {
        NHOWiringPi::ERROR_CODE = NHOWiringPi::MOTOR2_MISSING;
        return false;
    }
    
    return true;
}

/**
 * Forward
 **/
bool NHODEXM::forward() {

    // check motors
    if (! checkMotors()) {
        return false;
    }
    
    // attempt to execute
    if (!motorR->forward() || !motorL->forward()) {
        return false;
    }
    
    motion = true;
    
    // timer or not?
    if (TC->getDuration() > 0.0) {
        return startTimer(TC->getDuration());
    }
    
    return true;
}

bool NHODEXM::reverse() {
    
    // check motors
    if (! checkMotors()) {
        return false;
    }
    
   // attempt to execute
    if (!motorR->reverse() || !motorL->reverse()) {
        return false;
    }
    
    motion = true;

    // timer or not?
    if (TC->getDuration() > 0.0) {
        return startTimer(TC->getDuration());
    }

    return true;
}

bool NHODEXM::stopMotion() {
    
    // check motors
    if (! checkMotors()) {
        return false;
    }
    
    // attempt to execute
    if (!motorR->freeWheel() || !motorL->freeWheel()) {
        return false;
    }
    
    // attempt to execute
//    if (!motorR->stifle() || !motorL->stifle()) {
//        return false;
//    }
    
    motion = false;
    
    return true;
}

bool NHODEXM::turnLeft() {
    
    // check motors
    if (! checkMotors()) {
        return false;
    }
    
    // attempt to execute
    if (!motorR->forward() || !motorL->reverse()) {
        return false;
    }
    
    motion = true;
    
    // timer or not?
    if (TC->getDuration() > 0.0) {
        return startTimer(TC->getDuration());
    }
    
    return true;
}

bool NHODEXM::turnRight() {
    
    // check motors
    if (! checkMotors()) {
        return false;
    }
    
    // attempt to execute
    if (!motorR->reverse() || !motorL->forward()) {
        return false;
    }
    
    motion = true;
    
    // timer or not?
    if (TC->getDuration() > 0.0) {
        return startTimer(TC->getDuration());
    }
    
   return true;
}





