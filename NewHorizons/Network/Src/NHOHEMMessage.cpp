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
}

/**
 * Destructor
 **/
NHOHEMMessage::~NHOHEMMessage() {
    
//    delete data;
//    data = NULL;
    if (msg != NULL) {
        free(msg);
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
    memcpy((void *) (data + offset), HEMData->getPinModes(), sizeof(NHOWiringPi::GPIO_PINS)*sizeof(int));
    offset += sizeof(sizeof(NHOWiringPi::GPIO_PINS)*sizeof(int));
    
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
    int pins[NHOWiringPi::GPIO_PINS];
    memcpy(pins, (void *) (data + offset), sizeof(pins));
    offset += sizeof(pins);
    lData->setPinModes(pins);
    
    setHEMData(lData);
    
    return true;
}

