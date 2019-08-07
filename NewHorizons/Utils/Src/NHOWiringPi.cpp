//
//  NHOWiringPi.cpp
//  Utils
//
//  Created by Frédéric Rubio on 01/06/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include "NHOWiringPi.hpp"

#ifndef _RASPBIAN
static unsigned int modes[NHOWiringPi::GPIO_PINS];
static unsigned int analogValues[NHOWiringPi::GPIO_PINS];
static unsigned int digitalValues[NHOWiringPi::GPIO_PINS];
#endif

int NHOWiringPi::ERROR_CODE = 0;

const unsigned short NHOWiringPi::WiringPiMap[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};

struct NHOWiringPi::wiringPiNodeStruct *NHOWiringPi::wiringPiFindNode (int pin) {

    return NULL;
    
}

struct NHOWiringPi::wiringPiNodeStruct *NHOWiringPi::wiringPiNewNode  (int pinBase, int numPins) {
    
    return NULL;
    
}

void NHOWiringPi::wiringPiVersion    (int *major, int *minor) {
    
}

int NHOWiringPi::wiringPiSetup       (void) {
    
#ifdef _RASPBIAN
    return wiringPiSetup();
#else
    return 0;
#endif
    
}

int  NHOWiringPi::wiringPiSetupSys    (void) {
    
    return -1;
    
}

int  NHOWiringPi::wiringPiSetupGpio   (void) {
    
    return -1;

}

int  NHOWiringPi::wiringPiSetupPhys   (void) {
    
    return -1;

}

void NHOWiringPi::pinModeAlt          (int pin, int mode) {
    
}

void NHOWiringPi::pinMode             (int pin, int mode) {
   
#ifdef _RASPBIAN
    pinMode(pin, value);
#else
    modes[pin] = mode;
#endif

}

void NHOWiringPi::pullUpDnControl     (int pin, int pud) {
    
}

int  NHOWiringPi::digitalRead         (int pin) {
    
#ifdef _RASPBIAN
    return digitalRead(pin, value);
#else
    return digitalValues[pin];
#endif
    

}

void NHOWiringPi::digitalWrite        (int pin, int value) {

#ifdef _RASPBIAN
    digitalWrite(pin, value);
#else
    digitalValues[pin] = value;
#endif

}

unsigned int  NHOWiringPi::digitalRead8        (int pin) {

    return -1;

}

void NHOWiringPi::digitalWrite8       (int pin, int value) {
    
}

void NHOWiringPi::pwmWrite            (int pin, int value) {
    
}

int  NHOWiringPi::analogRead          (int pin) {
 
#ifdef _RASPBIAN
    return analogRead(pin, value);
#else
    return analogValues[pin];
#endif

}

int  NHOWiringPi::getAlt              (int pin) {
    
#ifdef _RASPBIAN
    return getAlt(pin, value);
#else
    return modes[pin];
#endif

}

void NHOWiringPi::analogWrite         (int pin, int value) {
    
#ifdef _RASPBIAN
    analogWrite(pin, value);
#else
    analogValues[pin] = value;
#endif
    

}


