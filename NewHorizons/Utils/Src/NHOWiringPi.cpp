//
//  NHOWiringPi.cpp
//  Utils
//
//  Created by Frédéric Rubio on 01/06/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include "NHOWiringPi.hpp"
#include "NHOLOG.hpp"
#include "wiringPi.h"

#include <string.h>
#include <iostream>

#ifndef _RASPBIAN
static unsigned int modes[NHOWiringPi::TOTAL_GPIO_PINS];
static unsigned int analogValues[NHOWiringPi::TOTAL_GPIO_PINS];
static unsigned int digitalValues[NHOWiringPi::TOTAL_GPIO_PINS];
#endif

int NHOWiringPi::ERROR_CODE = 0;

//const unsigned short NHOWiringPi::WiringPiMap[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};

struct NHOWiringPi::WiringPiNodeStruct *NHOWiringPi::WiringPiFindNode (int pin) {

    return NULL;
    
}

struct NHOWiringPi::WiringPiNodeStruct *NHOWiringPi::WiringPiNewNode  (int pinBase, int numPins) {
    
    return NULL;
    
}

void NHOWiringPi::WiringPiVersion    (int *major, int *minor) {
    
}

int NHOWiringPi::WiringPiSetup       (void) {
    
#ifdef _RASPBIAN
    return wiringPiSetup();
#else
    memset(modes, 0, sizeof(modes));
    memset(analogValues, 0, sizeof(modes));
    memset(digitalValues, 0, sizeof(modes));
    return 0;
#endif
    
}

int  NHOWiringPi::WiringPiSetupSys    (void) {
    
    return -1;
    
}

int  NHOWiringPi::WiringPiSetupGpio   (void) {
    
    return -1;

}

int  NHOWiringPi::WiringPiSetupPhys   (void) {
    
    return -1;

}

void NHOWiringPi::PinModeAlt          (int pin, int mode) {
    
}

void NHOWiringPi::PinMode             (int pin, int mode) {
   
#ifdef _RASPBIAN
    pinMode(pin, mode);
#else
    modes[pin] = mode;
#endif

}

void NHOWiringPi::PrintModes(const int pModes[]) {
#ifdef _RASPBIAN
#else
    std::string values = "";
    NHOFILE_LOG(logDEBUG) << "Modes: "  << std::endl ;
    for (int loop = 0 ; loop < NHOWiringPi::TOTAL_GPIO_PINS * 0.5; loop++) {
        values += std::to_string(pModes[loop]) + " ";
    }
    values += "\n";
    for (int loop = NHOWiringPi::TOTAL_GPIO_PINS * 0.5 ; loop < NHOWiringPi::TOTAL_GPIO_PINS; loop++) {
        values += std::to_string(pModes[loop]) + " ";
    }
    std::cout << values  << "\n";
#endif
}
void NHOWiringPi::PrintDigitalValues(const unsigned short pDigitalValues[]) {
#ifdef _RASPBIAN
#else
    std::string values = "";
    NHOFILE_LOG(logDEBUG) << "Digital values: "  << std::endl ;
    for (int loop = 0 ; loop < NHOWiringPi::TOTAL_GPIO_PINS * 0.5; loop++) {
        values += std::to_string(pDigitalValues[loop]) + " ";
    }
    values += "\n";
    for (int loop = NHOWiringPi::TOTAL_GPIO_PINS * 0.5 ; loop < NHOWiringPi::TOTAL_GPIO_PINS; loop++) {
        values += std::to_string(pDigitalValues[loop]) + " ";
    }
    std::cout << values  << "\n";
#endif
}

void NHOWiringPi::PrintModes() {
#ifdef _RASPBIAN
#else
    std::string values = "";
    NHOFILE_LOG(logDEBUG) << "Modes: "  << std::endl ;
    for (int loop = 0 ; loop < NHOWiringPi::TOTAL_GPIO_PINS * 0.5; loop++) {
        values += std::to_string(modes[loop]) + " ";
    }
    values += "\n";
    for (int loop = NHOWiringPi::TOTAL_GPIO_PINS * 0.5 ; loop < NHOWiringPi::TOTAL_GPIO_PINS; loop++) {
        values += std::to_string(modes[loop]) + " ";
    }
    std::cout << values  << "\n";
#endif
}

void NHOWiringPi::PrintDigitalValues() {
#ifdef _RASPBIAN
#else
    std::string values = "";
    NHOFILE_LOG(logDEBUG) << "Digital values: "  << std::endl ;
    for (int loop = 0 ; loop < NHOWiringPi::TOTAL_GPIO_PINS * 0.5; loop++) {
        values += std::to_string(digitalValues[loop]) + " ";
    }
    values += "\n";
    for (int loop = NHOWiringPi::TOTAL_GPIO_PINS * 0.5 ; loop < NHOWiringPi::TOTAL_GPIO_PINS; loop++) {
        values += std::to_string(digitalValues[loop]) + " ";
    }
    std::cout << values  << "\n";
#endif
}

void NHOWiringPi::PullUpDnControl     (int pin, int pud) {
    
}

int  NHOWiringPi::DigitalRead         (int pin) {
    
#ifdef _RASPBIAN
    return digitalRead(pin);
#else
    return digitalValues[pin];
#endif
    

}

void NHOWiringPi::DigitalWrite        (int pin, int value) {

#ifdef _RASPBIAN
    digitalWrite(pin, value);
#else
    digitalValues[pin] = value;
#endif

}

unsigned int  NHOWiringPi::DigitalRead8        (int pin) {

    return -1;

}

void NHOWiringPi::DigitalWrite8       (int pin, int value) {
    
}

void NHOWiringPi::PwmWrite            (int pin, int value) {
    
}

int  NHOWiringPi::AnalogRead          (int pin) {
 
#ifdef _RASPBIAN
    return analogRead(pin);
#else
    return analogValues[pin];
#endif

}

int  NHOWiringPi::GetAlt              (int pin) {
    
#ifdef _RASPBIAN
    return getAlt(pin);
#else
    return modes[pin];
#endif

}

void NHOWiringPi::AnalogWrite         (int pin, int value) {
    
#ifdef _RASPBIAN
    analogWrite(pin, value);
#else
    analogValues[pin] = value;
#endif
    

}


