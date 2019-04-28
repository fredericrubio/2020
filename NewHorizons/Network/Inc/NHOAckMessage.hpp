//
//  NHOAckMessage.hpp
//  Network
//
//  Created by Frédéric Rubio on 28/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOAckMessage_hpp
#define NHOAckMessage_hpp

class NHOAckMessage {
    
    public :
    NHOAckMessage() {};
    NHOAckMessage(const unsigned int pSize):value(pSize) {};
    
    bool serialize(char * const);
    
    bool unserialize(const char *const);
    
    virtual unsigned int getSize();
    
    unsigned int getValue() {return value;};
    
    protected :
    unsigned int value;
};
#endif /* NHOAckMessage_hpp */
