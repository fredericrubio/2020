#ifdef ESP32_ADAFRUIT_FEATHER
#include <Arduino.h>
#endif

#include <iostream>
#include <stdio.h>
#ifdef ESP32_ADAFRUIT_FEATHER
#include "NHOSolenoidValveData.hpp"
#include "Network/NHOMessageFactory.hpp"
#include "NHOHEMData.hpp"
#include "Utils/NHOLOG.hpp"
#else
#include "NHOSolenoidValveData.hpp"
#include "NHOMessageFactory.hpp"
#include "NHOHEMData.hpp"
#include "NHOLOG.hpp"
#endif

NHOSolenoidValveData::NHOSolenoidValveData(const NHOSolenoidValvePayload pCommand, const long long pDate) {
    
    this->type = NHOMessageFactory::eSolenoidValve;
    this->date = pDate;
    this->command = pCommand;
    
}

NHOSolenoidValveData::NHOSolenoidValveData() {
    
    type = NHOMessageFactory::eSolenoidValve;
    this->date = clock();
    command = eUnknown;
    
}

/**
 * Copy constructor.
 **/
NHOSolenoidValveData::NHOSolenoidValveData(const NHOSolenoidValveData* orig) {
    
    this->type = NHOMessageFactory::eSolenoidValve;
    
    this->date = orig->date;
    this->command = orig->command;
    
}

/**
 * Copy constructor.
 **/
NHOSolenoidValveData::NHOSolenoidValveData(const NHOSolenoidValveData& orig) {
    
    this->type = NHOMessageFactory::eSolenoidValve;
    
    this->date = orig.date;
    this->command = orig.command;
    
}

NHOSolenoidValveData::~NHOSolenoidValveData() {
}


