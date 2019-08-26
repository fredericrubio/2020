/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HEM_Data.h
 * Author: fredericrubio
 *
 * Created on 14 octobre 2018, 21:52
 */

#ifndef NHOHEMDATA_H
#define NHOHEMDATA_H

#include <string.h>

#include "NHOSensorData.hpp"
#include "NHOWiringPi.hpp"

class NHOHEMData : public NHOSensorData {
    
public:    
    NHOHEMData();
    NHOHEMData(const long long pDate);
    /**
     * Copy constructor.
     **/
    NHOHEMData(const NHOHEMData& orig);
    NHOHEMData(const NHOHEMData* orig);
    virtual ~NHOHEMData();
    
    /**
     * Returns the size (in bytes) of a HEM message
     **/
    static inline size_t getSize() {
        return (sizeof(date) + sizeof(cpu) + sizeof(temp) + sizeof(usedMemory) + sizeof(modes) + sizeof(digitalValues)) ;
    }
    
    /**
     * Getters
     **/
    inline short getCPUUsage() const {return cpu;};
    inline short getTemperature() const {return temp;};
    inline short getMemoryUsage() const {return usedMemory;};
    inline const int* const getPinModes() const {return modes;};
    inline const unsigned short* const getDigitalValues() const {return digitalValues;};

    /**
     * Setters
     **/
    inline void setCPUUsage(const short pCPU) {cpu = pCPU;};
    inline void setTemperature(const short pTemperature) {temp = pTemperature;};
    inline void setMemoryUsage(const short pMemory) {usedMemory = pMemory;};
    inline void setDate(const long long pDate) {date = pDate;};
    inline void setPinModes(const int* pModes) {memcpy(modes, pModes, sizeof(modes));};
    inline void setDigitalValues(const unsigned short* pDigitalValues) {memcpy(digitalValues, pDigitalValues, sizeof(digitalValues));};

    /**
     * Update health monitoring sata
     **/
    bool fetch();
    
private: 
    long long   date; // 'long long' to force 64bits on 32bits OS
    
    short cpu;
    short temp;
    short usedMemory;
    int   modes[NHOWiringPi::TOTAL_GPIO_PINS]; // INPUT or OUTPUT
    int analogValues[NHOWiringPi::TOTAL_GPIO_PINS];
    unsigned short digitalValues[NHOWiringPi::TOTAL_GPIO_PINS]; // HIGH or LOW

    /**
     * Fetch CPU
     **/
    bool fetchCPUUsage();
    
    /**
     * Fetch Temperature
     **/
    bool fetchTemperature();
    
    /**
     * Fetch Memory
     **/
    bool fetchMemoryUsage();
    
    /**
     * Fetch Pins
     **/
    bool fetchPins();
};

#endif /* NHOHEMDATA_H */

