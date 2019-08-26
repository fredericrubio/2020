//
//  NHOTCMessage.hpp
//  Network
//
//  Created by Frédéric Rubio on 02/06/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOTCMessage_hpp
#define NHOTCMessage_hpp

#include "NHOMessage.hpp"

class NHOTCMessage : public NHOMessage {
    
public:
    
    /**
     * All types of commands
     **/
    typedef enum {eUnknown, eForward, eReverse, eTurnLeft, eTurnRight, eStop} NHOCommand ;
    
    NHOTCMessage(long long pDate);
    
    /**
     * Copy constructor
     **/
    NHOTCMessage(const NHOTCMessage & pValue);

    /**
     * Desctructor
     **/
    ~NHOTCMessage();
    
    /**
     * Serializes the current values of the health monitoring message (allocates and fills the
     * the buffer readdy to be sent.
     **/
    bool serialize();
    
    /**
     * Unserializes the buffer into the attributes.
     * Returns false if buffer not initialized.
     * The management of values out of definition ranges is still to be tackled.
     **/
    bool unserialize();
    
    /**
     * Compute the size of the current message.
     **/
    virtual unsigned int computeSize();
    
    /**
     * Getters
     **/
    inline NHOCommand getCommand() const { return command;};
    inline float getMagnitude() const { return magnitude;};
    inline float getDuration() const { return duration;};
    inline float getSpeed() const { return speed;};

    /**
     * Setters
     **/
    inline void setCommand(const NHOCommand pCommand) {command = pCommand;};
    inline void setMagnitude(const float pValue) {magnitude = pValue;};
    inline void setDuration(const float pValue) {duration = pValue;};
    inline void setSpeed(const float pValue) {speed = pValue;};

protected :
    NHOCommand  command;
    float       magnitude;
    float       duration;
    float       speed;
    
};
#endif /* NHOTCMessage_hpp */
