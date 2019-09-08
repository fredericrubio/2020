//
//  NHOMotor.cpp
//  CommandCenter
//
//  Created by Frédéric Rubio on 10/06/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include "NHOMotor.hpp"

#include "NHOLOG.hpp"
#include "NHOWiringPi.hpp"

/**
 * Constructor
 **/
NHOMotor::NHOMotor(const std::string& pName, unsigned short pPort1, unsigned short pPort2, unsigned short pPort3):
name(pName), port1(pPort1), port2(pPort2), port3(pPort3), speed(0) {
    
    // three values to test
    // OUTPUT: for first tests
    // PWM_OUTPUT: only with wiringPi pins 1 and 23
    // SOFT_PWM_OUTPUT: for any others pins but requires softPwmWrite and may be softPwmStop
    NHOWiringPi::PinMode(port1, NHOWiringPi::OUTPUT);
    NHOWiringPi::PinMode(port2, NHOWiringPi::OUTPUT);
    NHOWiringPi::PinMode(port3, NHOWiringPi::OUTPUT);

}

// -- Commands --
/**
 * Forward
 **/
bool NHOMotor::forward(){
    
    NHOWiringPi::DigitalWrite(port1, NHOWiringPi::HIGH);
    NHOWiringPi::DigitalWrite(port2, NHOWiringPi::HIGH);
    NHOWiringPi::DigitalWrite(port3, NHOWiringPi::LOW);

    NHOFILE_LOG(logDEBUG) << "NHOMotor::forward " << name << " forwarding." << std::endl;

    NHOWiringPi::PrintDigitalValues();
    
    return true;
    
}

/**
 * Reverse
 **/
bool NHOMotor::reverse(){
    
    NHOWiringPi::DigitalWrite(port1, NHOWiringPi::HIGH);
    NHOWiringPi::DigitalWrite(port2, NHOWiringPi::LOW);
    NHOWiringPi::DigitalWrite(port3, NHOWiringPi::HIGH);

    NHOFILE_LOG(logDEBUG) << "NHOMotor::reverse " << name << " reversing." << std::endl;
    
    return true;
    
}

/**
 * Break (fast stop)
 **/
bool NHOMotor::stifle() {

    NHOWiringPi::DigitalWrite(port1, NHOWiringPi::LOW);
    NHOWiringPi::DigitalWrite(port2, NHOWiringPi::LOW);
    NHOWiringPi::DigitalWrite(port3, NHOWiringPi::LOW);

    NHOFILE_LOG(logDEBUG) << "NHOMotor::stifle " << name << "." << std::endl;
    
    return true;
    
}

/**
 * Free wheel
 **/
bool NHOMotor::freeWheel() {

    NHOWiringPi::DigitalWrite(port1, NHOWiringPi::HIGH);
    NHOWiringPi::DigitalWrite(port2, NHOWiringPi::LOW);
    NHOWiringPi::DigitalWrite(port3, NHOWiringPi::LOW);
    
    NHOFILE_LOG(logDEBUG) << "NHOMotor::freeWheel " << name << "." << std::endl;

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

