//
//  NHOImageSizeMessageBody.cpp
//  ComLib
//
//  Created by Frédéric Rubio on 04/09/2018.
//  Copyright © 2018 Frédéric Rubio. All rights reserved.
//

#include <string.h>
#include <stdlib.h>

#include "NHOHallEffectMessage.hpp"
#include "NHOHallEffectData.hpp"
/**
 * Constructors
 **/
NHOHallEffectMessage::NHOHallEffectMessage(long long pDate):
NHOMessage(pDate, NHOMessageFactory::eHallEffect) {
    this->HEData = new NHOHallEffectData(-1);
    computeSize();
}

NHOHallEffectMessage::NHOHallEffectMessage(const NHOHallEffectMessage* pMsg) :
NHOMessage(pMsg->getDate(), NHOMessageFactory::eHallEffect) {
    this->HEData = new NHOHallEffectData(-1);
    if (pMsg->getData() != NULL) {
        this->size = pMsg->getSize();
        this->data = (char *) calloc(this->getSize(), sizeof(char));
        memcpy((void *) this->data, (void *) pMsg->getData(), pMsg->getSize());
    }
    this->unserialize();
}

NHOHallEffectMessage::NHOHallEffectMessage(const NHOHallEffectMessage& pMsg) :
NHOMessage(pMsg.getDate(), NHOMessageFactory::eHallEffect) {
    this->HEData = new NHOHallEffectData(-1);
    if (pMsg.getData() != NULL) {
        this->size = pMsg.getSize();
        this->data = (char *) calloc(this->getSize(), sizeof(char));
        memcpy((void *) this->data, (void *) pMsg.getData(), pMsg.getSize());
    }
    this->unserialize();
}

/**
 * Destructor
 **/
NHOHallEffectMessage::~NHOHallEffectMessage() {
    if (this->HEData != NULL) {
        delete this->HEData;
    }
}

unsigned int NHOHallEffectMessage::computeSize() {
    if (size != 0) {
        return size;
    }
    size = getHeaderSize();
    size += this->HEData->getSize();
    return size;
}

bool NHOHallEffectMessage::serialize() {
    if (this->data != NULL) {
        free(this->data);
    }
    
    // mempry allocation
    this->data = (char *) calloc(this->getSize(), sizeof(char));
    unsigned long offset = this->serializeHeader();
    // copy values
    /// date from data
    long long lDate = this->HEData->getDate();
    memcpy((void *) (this->data + offset), &(lDate), sizeof(lDate));
    offset += sizeof(lDate);
    // NHOFILE_LOG(logINFO) << "NHOHallEffectMessage::serialize - offset: " << offset << ".\n";
    /// hall effect
    short lHE = this->HEData->getHallEffect();
    memcpy((void *) (this->data + offset), &(lHE), sizeof(lHE));
    offset += sizeof(lHE);
    // NHOFILE_LOG(logINFO) << "NHOHallEffectMessage::serialize - offset: " << offset << ".\n";
    
    return true;
    
}

bool NHOHallEffectMessage::unserialize() {
    
    if (data == NULL) {
        return false;
    }
    
    // unserialize header
    unsigned int offset = unserializeHeader() ;
    
    // copy values
    /// date
    long long lDate;
    memcpy(&lDate, (void *) data, sizeof(lDate));
    offset += sizeof(lDate);
    this->getHEData()->setDate(lDate);
    
    /// Hall Effect
    short lHE;
    memcpy(&lHE, (void *) (data + offset), sizeof(lHE));
    offset += sizeof(lHE);
    this->getHEData()->setHallEffect(lHE);
    
    return true;
    
    
}
