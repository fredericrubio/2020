//
//  NHOAckMessage.hpp
//  Network
//
//  Created by Frédéric Rubio on 28/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOAckMessage_hpp
#define NHOAckMessage_hpp

#include "NHOMessage.hpp"

class NHOAckMessage : public NHOMessage {
    
    public :
        /**
         * Constructors
         **/
        NHOAckMessage(long long pDate);
        NHOAckMessage(long long pDate, const unsigned int pSize);
        /**
         * Destructor
         **/
        virtual ~NHOAckMessage();
    
        virtual bool serialize();
        virtual bool unserialize();
    
        virtual unsigned int computeSize();
    
        unsigned int getValue() {return value;};
    
    protected :
        unsigned int value;
    
};
#endif /* NHOAckMessage_hpp */
