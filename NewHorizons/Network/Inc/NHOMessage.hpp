//
//  NHOMessage.hpp
//  ComLib
//
//  Created by Frédéric Rubio on 04/09/2018.
//  Copyright © 2018 Frédéric Rubio. All rights reserved.
//

#ifndef IMP_Message_hpp
#define IMP_Message_hpp

#include <stdio.h>
#include <string.h>

#include "NHOMessageFactory.hpp"

class NHOMessage {
    
    public :
    /**
     * Constructors
     **/
    NHOMessage(long long pDate, NHOMessageFactory::NHOMessageType pType);

    /**
     * Destructor
     **/
    virtual ~NHOMessage();
    
    virtual bool serialize() = 0;
    virtual bool unserialize() = 0;

    const char* getData() const {return data;};

    virtual unsigned int getSize() const {return size;};
    virtual unsigned int computeSize() = 0;    

    virtual const unsigned short getHeaderSize() ;

    virtual const unsigned int serializeHeader();
    virtual const unsigned int unserializeHeader();
    
    /////////////////////////////////////////
    // Constants and static methods
    // Maximum size of a message (to ease the reception of a message we don't know the type)
    static const unsigned int MAX_SIZE = 255;
    
    /**
     * Extract the message type from the char array.
     **/
    static NHOMessageFactory::NHOMessageType getType(const char * const pMessage) {
        unsigned int lOffset = 0;
        NHOMessageFactory::NHOMessageType type;
        lOffset = sizeof(date);
        memcpy(&type, pMessage + lOffset, sizeof(NHOMessageFactory::NHOMessageType));
        return type;  
    }
    
    virtual inline void setData(const char* const pData) {data = (char *) pData;};
    
    /**
     * Extract the message date from the char array.
     **/
    static long long getDate(const char * const pMessage) {
        unsigned int lOffset = 0;
        unsigned long lDate;
        memcpy(&lDate, pMessage + lOffset, sizeof(unsigned long));
        return lDate;
    }
    
    inline long long getDate() const {return date;};
    // Constants and static methods
    /////////////////////////////////////////

    protected :
        NHOMessageFactory::NHOMessageType type;

        long long   date; // 'long long' to force 64bits on 32bits OS

        char* data;

        unsigned int size;
};
#endif /* IMP_Message_hpp */
