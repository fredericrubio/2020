//
//  NHOImageSizeMessageBody.cpp
//  ComLib
//
//  Created by Frédéric Rubio on 04/09/2018.
//  Copyright © 2018 Frédéric Rubio. All rights reserved.
//
#ifdef ESP32_ADAFRUIT_FEATHER
#include <Arduino.h>
#include "NHOPlatform.hpp"
#endif

#include <string.h>
#include <stdlib.h>

#ifdef ESP32_ADAFRUIT_FEATHER     
    #include "esp_heap_caps.h"
    #include "Utils/NHOLOG.hpp"
#else
    #include "NHOLOG.hpp"
#endif
  
#include "NHOSolenoidValveMessage.hpp"


/**
 * Constructors
 **/
NHOSolenoidValveMessage::NHOSolenoidValveMessage(long long pDate)
:NHOMessage(pDate, NHOMessageFactory::eSolenoidValve) {
    this->date = pDate;
    this->data = NULL;
    this->solenoidValveData = new NHOSolenoidValveData();
    computeSize();
}

NHOSolenoidValveMessage::NHOSolenoidValveMessage(const NHOSolenoidValveMessage* pMsg) :
NHOMessage(pMsg->getDate(), NHOMessageFactory::eSolenoidValve) {
    this->solenoidValveData = new NHOSolenoidValveData(pMsg->getSoleniodValveData()->getCommand(), clock());
    this->computeSize();
}

/**
 * Destructor
 **/
NHOSolenoidValveMessage::~NHOSolenoidValveMessage() {  
    if (this->solenoidValveData != NULL) {
        delete this->solenoidValveData;
    }
}

unsigned int NHOSolenoidValveMessage::computeSize() {
    if (size != 0) {
        return size;
    }
    
    size = getHeaderSize();
    // NHOFILE_LOG(logINFO) << "NHOSolenoidValveMessage::computeSize - header size: " << this->getHeaderSize() << ".\n";
    size += NHOSolenoidValveData::getSize();
    // NHOFILE_LOG(logINFO) << "NHOSolenoidValveMessage::computeSize - data size: " << this->HEData->getSize() << ".\n";
    return size;
}

bool NHOSolenoidValveMessage::serialize() {
    
    if (this->data != NULL) {
        free(this->data);
    }
    
    // mempry allocation
#ifdef ESP32_ADAFRUIT_FEATHER     
    this->data = (char *) heap_caps_malloc(this->getSize() * sizeof(char), MALLOC_CAP_DEFAULT);
    // NHOFILE_LOG(logINFO) << "NHOSolenoidValveMessage::serialize - size: " << this->getSize() << ".\n";
#else
    this->data = (char *) calloc(this->getSize(), sizeof(char));
#endif    
    if (this->data == NULL) {
        NHOFILE_LOG(logINFO) << "NHOSolenoidValveMessage::serialize allocation failure.\n";
        return false;
    }    
    unsigned long offset = this->serializeHeader();
    // copy values
    /// date from data
    long long lDate = this->solenoidValveData->getDate();
    memcpy((void *) (this->data + offset), &(lDate), sizeof(lDate));
    offset += sizeof(lDate);
    // NHOFILE_LOG(logINFO) << "NHOSolenoidValveMessage::serialize - offset: " << offset << ".\n";
    /// hall effect
    NHOSolenoidValveData::NHOSolenoidValvePayload lCommand = this->solenoidValveData->getCommand();
    memcpy((void *) (this->data + offset), &(lCommand), sizeof(lCommand));
    // NHOFILE_LOG(logINFO) << "NHOSolenoidValveMessage::serialize - offset: " << offset << ".\n";
    
    return true;
}

bool NHOSolenoidValveMessage::unserialize() {
  
    // unsigned long offset = 0;
    // memcpy(&date, (void *) data, sizeof(date));
    // offset += sizeof(date);


    if (data == NULL) {
        return false;
    }
    // NHOFILE_LOG(logDEBUG) << "\nNHOSolenoidValveMessage::unserialize.\n";

    // unserialize header
    unsigned long offset = unserializeHeader() ;
    // NHOFILE_LOG(logDEBUG) << "\nNHOSolenoidValveMessage::unserialize  - date <" << this->date << ">.\n";
    // NHOFILE_LOG(logDEBUG) << "\nNHOSolenoidValveMessage::unserialize  - type <" << this->type << ">.\n";

    // copy values
    /// date
    long long lDate;
    // NHOFILE_LOG(logDEBUG) << "\nNHOSolenoidValveMessage::unserialize - sizeof(long long) <" << sizeof(lDate) << ">.\n";
    // NHOFILE_LOG(logDEBUG) << "\nNHOSolenoidValveMessage::unserialize - offset <" << offset << ">.\n";
    memcpy(&lDate, (void *) (this->data + offset), sizeof(lDate));
    offset += sizeof(lDate);
    this->getSoleniodValveData()->setDate(lDate);
    
    // NHOFILE_LOG(logDEBUG) << "\nNHOSolenoidValveMessage::unserialize - finish.\n";

    ///
    NHOSolenoidValveData::NHOSolenoidValvePayload lPayload;
    memcpy(&lPayload, (void *) (this->data + offset), sizeof(lPayload));
    offset += sizeof(lPayload);
    this->getSoleniodValveData()->setCommand(lPayload);
    
    return true;
    
}
