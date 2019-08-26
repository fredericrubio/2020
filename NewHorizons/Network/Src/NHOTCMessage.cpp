//
//  NHOTCMessage.cpp
//  Network
//
//  Created by Frédéric Rubio on 02/06/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include <string.h>
#include <stdlib.h>

#include "NHOTCMessage.hpp"

NHOTCMessage::NHOTCMessage(long long pDate):
NHOMessage(pDate, NHOMessageFactory::eTC){
    
    command = eUnknown;
    magnitude = -1.0;
    duration = -1.0;
    speed = -1.0;
    
}

/**
 * Copy constructor
 **/
NHOTCMessage::NHOTCMessage(const NHOTCMessage & pValue):
NHOMessage(pValue.getDate(), NHOMessageFactory::eTC) {
    
    command = pValue.getCommand();
    magnitude = pValue.getMagnitude();
    duration = pValue.getDuration();
    speed = pValue.getSpeed();

}

/**
 * Desctructor
 **/
NHOTCMessage::~NHOTCMessage() {

    
    
}

/**
 * Serializes the current values of the health monitoring message (allocates and fills the
 * the buffer readdy to be sent.
 **/
bool NHOTCMessage::serialize() {
    
    if (data != NULL) {
        free(data);
    }
    
    // update size
    computeSize();
    
    // mempry allocation
    data = (char *) calloc(getSize(), sizeof(char));
    
    // header
    unsigned int offset = serializeHeader();

    // copy values
    /// command
    NHOCommand lCommand = getCommand();
    memcpy((void *) (data + offset), &lCommand, sizeof(lCommand));
    offset += sizeof(lCommand);
    
    /// magnitude
    float lMagnitude = getMagnitude();
    memcpy((void *) (data + offset), &lMagnitude, sizeof(lMagnitude));
    offset += sizeof(lMagnitude);
    
    /// duration
    float lDuration = getDuration();
    memcpy((void *) (data + offset), &lDuration, sizeof(lDuration));
    offset += sizeof(lDuration);
    
    /// speed
    float lSpeed = getSpeed();
    memcpy((void *) (data + offset), &lSpeed, sizeof(lSpeed));
    offset += sizeof(lSpeed);
    
    return true;
}

/**
 * Unserializes the buffer into the attributes.
 * Returns false if buffer not initialized.
 * The management of values out of definition ranges is still to be tackled.
 **/
bool NHOTCMessage::unserialize() {
    
    if (data == NULL) {
        return false;
    }
    
    // unserialize header
    unsigned int offset = unserializeHeader() ;
    
    NHOCommand  lCommand;
    float       lMagnitude;
    float       lDuration;
    float       lSpeed;

    // command
    memcpy(&lCommand, (void *) (data + offset), sizeof(lCommand));
    offset += sizeof(lCommand);
    // magnitude
    memcpy(&lMagnitude, (void *) (data + offset), sizeof(lMagnitude));
    offset += sizeof(lMagnitude);
    // duration
    memcpy(&lDuration, (void *) (data + offset), sizeof(lDuration));
    offset += sizeof(lDuration);
    // speed
    memcpy(&lSpeed, (void *) (data + offset), sizeof(lSpeed));
    offset += sizeof(lSpeed);

    // set values
    setCommand(lCommand);
    setMagnitude(lMagnitude);
    setDuration(lDuration);
    setSpeed(lSpeed);
    
    return true;

}

/**
 * Compute the size of the current message.
 **/
unsigned int NHOTCMessage::computeSize() {
    
    if (size != 0) {
        return size;
    }
    
    size = getHeaderSize();
    size += sizeof(command);
    size += sizeof(magnitude);
    size += sizeof(duration);
    size += sizeof(speed);
    return size;

}
