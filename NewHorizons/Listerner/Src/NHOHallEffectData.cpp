#ifdef ESP32_ADAFRUIT_FEATHER
    #include <Arduino.h>
#endif

#include <iostream>
#include <stdio.h>
#include "NHOHallEffectData.hpp"
#include "NHOMessageFactory.hpp"
#include "NHOHEMData.hpp"
#include "NHOLOG.hpp"

/**
 *
 **/
NHOHallEffectData::NHOHallEffectData(const int pinNumber) {
    
    type = NHOMessageFactory::eHallEffect;
    
    this->date = 0;
    this->hallEffect = 0;
    this->valuePin = pinNumber;
#ifdef ESP32_ADAFRUIT_FEATHER
    pinMode(this->valuePin, INPUT);
#endif
}

NHOHallEffectData::NHOHallEffectData(const int pinNumber, const long long pDate){
    
    type = NHOMessageFactory::eHallEffect;

    this->hallEffect = 0;
    this->valuePin = pinNumber;
#ifdef ESP32_ADAFRUIT_FEATHER
    pinMode(this->valuePin, INPUT);
#endif
    this->setDate(pDate);
}

/**
 * Copy constructor.
 **/
NHOHallEffectData::NHOHallEffectData(const NHOHallEffectData* orig) {
    
    this->type = NHOMessageFactory::eHallEffect;
    
    this->date = orig->date;
    this->hallEffect = orig->hallEffect;
    this->valuePin = orig->getPinNumber();

}

/**
 * Copy constructor.
 **/
NHOHallEffectData::NHOHallEffectData(const NHOHallEffectData& orig) {
    
    this->type = NHOMessageFactory::eHallEffect;
    
    this->date = orig.date;
    this->hallEffect = orig.hallEffect;
    this->valuePin = orig.getPinNumber();
    
}

NHOHallEffectData::~NHOHallEffectData() {
}

/**
 * Update health monitoring sata
 **/
bool NHOHallEffectData::fetch() {
    bool lReturn = false;
    lReturn = fetchHallEffect();   
    return lReturn;
}
    
/**
 * Fetch CPU
 **/
bool NHOHallEffectData::fetchHallEffect() {
    this->date = clock();
#ifdef ESP32_ADAFRUIT_FEATHER
    this->hallEffect = digitalRead(valuePin);
#else
    this->hallEffect = 2022;
#endif
    return (this->hallEffect != 0);
}


