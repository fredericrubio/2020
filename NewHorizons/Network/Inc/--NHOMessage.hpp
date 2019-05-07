//
//  NHOMessage.hpp
//  Network
//
//  Created by Frédéric Rubio on 22/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOMessage_hpp
#define NHOMessage_hpp

#include <stddef.h>
#include <stdint.h>

class NHOData;

class NHOMessage {
    
public:
    
    /**
     * Constructor
     **/
    NHOMessage(const long long   pDate);
    
    /**
     * Destructor
     **/
    ~NHOMessage();
    
    virtual inline size_t getSize() const { return size; };

    virtual inline unsigned char* getData() const {return data;};
    
protected :
    // size of the message
    size_t size;
    
    // contents of the message
    unsigned char* data;
    
    long long   date; // 'long long' to force 64bits on 32bits OS
};
#endif /* NHOMessage_hpp */
