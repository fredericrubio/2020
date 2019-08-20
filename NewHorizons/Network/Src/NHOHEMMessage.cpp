//
//  HEM_Message.cpp
//  ComLib
//
//  Created by Frédéric Rubio on 13/10/2018.
//  Copyright © 2018 Frédéric Rubio. All rights reserved.
//
#include <string.h>
#include <iostream>

#include "NHOHEMMessage.hpp"

#include "NHOHEMData.hpp"
#include "NHOMessageFactory.hpp"

/**
 * Constructors
 **/
NHOHEMMessage::NHOHEMMessage(const long long   pDate):
NHOMessage(pDate, NHOMessageFactory::eHEM){
    date = pDate;
    data = NULL;
    msg = NULL;
    HEMData = new NHOHEMData();
}

/**
 * Copy constructor
 **/
NHOHEMMessage::NHOHEMMessage(const NHOHEMMessage & pValue):
NHOMessage(pValue.getDate(), NHOMessageFactory::eHEM) {
    
    data = NULL;
    msg = NULL;
    if (pValue.getHEMData()) {
        HEMData = new NHOHEMData(pValue.getHEMData());
    }
    else {
        HEMData = new NHOHEMData();
    }
    
}

/**
 * Destructor
 **/
NHOHEMMessage::~NHOHEMMessage() {
    
    if (this->HEMData != NULL) {
        delete this->HEMData;
    }
    if (this->data != NULL) {
        free(this->data);
        this->data = NULL;
    }
    if (this->msg != NULL) {
        free(this->msg);
    }
    
}


unsigned int NHOHEMMessage::computeSize() {
    
    size = 0;
    
    return size;
    
}

/**
 * Serializes the current values of the health monitoring message (allocates and fills the
 * the buffer readdy to be sent.
 **/
bool NHOHEMMessage::serialize() {
    
    if (HEMData == NULL) {
        return false;
    }
    
    if (data != NULL) {
        free(data);
    }
    
    // calculate message size
    size_t lSize = HEMData->getSize();

    data = (char *) calloc(lSize, sizeof(char));
    
    // copy values
    unsigned int offset = 0;
    /// date
    long long lDate = HEMData->getDate();
    memcpy((void *) data, &lDate, sizeof(lDate));
    offset += sizeof(lDate);
    /// cpu
    short lCpu = HEMData->getCPUUsage();
    memcpy((void *) (data + offset), &lCpu, sizeof(lCpu));
    offset += sizeof(lCpu);
    
    /// temp
    short lTemp = HEMData->getTemperature();
    memcpy((void *) (data + offset), &lTemp, sizeof(lTemp));
    offset += sizeof(lTemp);
    
    /// mempry
    short lMemory = HEMData->getMemoryUsage();
    memcpy((void *) (data + offset), &lMemory, sizeof(lMemory));
    offset += sizeof(lMemory);
    
    /// pins
    //// modes
    memcpy((void *) (data + offset), HEMData->getPinModes(), NHOWiringPi::TOTAL_GPIO_PINS * sizeof(int));
    offset += NHOWiringPi::TOTAL_GPIO_PINS * sizeof(int);
    
    //// digital values
    memcpy((void *) (data + offset), HEMData->getDigitalValues(), NHOWiringPi::TOTAL_GPIO_PINS * sizeof(unsigned short));
    offset += NHOWiringPi::TOTAL_GPIO_PINS * sizeof(unsigned short);
    
//    NHOWiringPi::printModes((int *) HEMData->getPinModes());
//    NHOWiringPi::printDigitalValues((unsigned short *) HEMData->getDigitalValues());

    size = (unsigned int) lSize;
    return true;
}

/**
 * Unserializes the buffer into the attributes.
 * Returns false if buffer not initialized.
 * The management of values out of definition ranges is still to be tackled.
 **/
bool NHOHEMMessage::unserialize() {
    // calculate message size
    unsigned int offset = 0;
    NHOHEMData* lData = new NHOHEMData();
    
    if (data == NULL) {
        // no data to decod
        delete lData;
        return false;
    }
    
    // copy values
    /// date
    long long lDate;
    memcpy(&lDate, (void *) data, sizeof(lDate));
    offset += sizeof(lDate);
    lData->setDate(lDate);
    
    /// cpu
    short lCpu;
    memcpy(&lCpu, (void *) (data + offset), sizeof(lCpu));
    offset += sizeof(lCpu);
    lData->setCPUUsage(lCpu);
    
    /// temp
    short lTemperature;
    memcpy(&lTemperature, (void *) (data + offset), sizeof(lTemperature));
    offset += sizeof(lTemperature);
    lData->setTemperature(lTemperature);
    
    /// memory
    short lMemory;
    memcpy(&lMemory, (void *) (data + offset), sizeof(lMemory));
    offset += sizeof(lMemory);
    lData->setMemoryUsage(lMemory);
    
    /// pins
    int pins[NHOWiringPi::TOTAL_GPIO_PINS];
    memcpy(pins, (void *) (data + offset), sizeof(pins));
    offset += sizeof(pins);
    lData->setPinModes(pins);

    unsigned short digitalValues[NHOWiringPi::TOTAL_GPIO_PINS];
    memcpy(digitalValues, (void *) (data + offset), sizeof(digitalValues));
    offset += sizeof(digitalValues);
    lData->setDigitalValues(digitalValues);

//    NHOWiringPi::printModes(pins);
//    NHOWiringPi::printDigitalValues(digitalValues);

    setHEMData(lData);
    
    return true;
}

