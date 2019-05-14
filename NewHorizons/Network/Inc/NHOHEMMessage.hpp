//
//  HEM_Message.hpp
//  ComLib
//
//  Created by Frédéric Rubio on 13/10/2018.
//  Copyright © 2018 Frédéric Rubio. All rights reserved.
//

#ifndef NHOHEMMessage_hpp
#define NHOHEMMessage_hpp

#include <stdio.h>
#include "NHOMessage.hpp"

class NHOHEMData;

class NHOHEMMessage : public NHOMessage {
  
public:
    /**
     * Constructor
     **/
    NHOHEMMessage(const long long   pDate);
    
    /**
     * Destructor
     **/
    ~NHOHEMMessage();
   
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
    
    virtual unsigned int computeSize();
    
    /**
     * Set the payload.
     **/
    inline void setHEMData(NHOHEMData* const pData) {HEMData = pData;};
    /**
     * Get the payload
     **/
    inline const NHOHEMData* const getHEMDate() const {return HEMData;};
    
    inline const char* getMsg() const {return msg;};
    inline void setMsg(const int pSize, const char* pMsg) {
        if (msg != NULL) {
            free(msg);
        }
        msg = (char *) calloc(pSize, 0); memcpy(msg, pMsg, pSize);
    };

private:
    long long   date; // 'long long' to force 64bits on 32bits OS
    
    NHOHEMData*   HEMData;
    
    char* msg;

    
};
#endif /* NHOHEMMessage_hpp */
