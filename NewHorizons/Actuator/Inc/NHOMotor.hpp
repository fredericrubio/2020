//
//  NHOMotor.hpp
//  CommandCenter
//
//  Created by Frédéric Rubio on 10/06/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOMotor_hpp
#define NHOMotor_hpp

class NHOMotor {
    
public:
    
    /**
     * Constructor
     **/
    NHOMotor(unsigned short pPort1, unsigned short pPort2, unsigned short pPort3);
    
    // -- Commands --
    /**
     * Forward
     **/
    bool forward();

    /**
     * Reverse
     **/
    bool reverse();
    
    /**
     * Break (fast stop)
     **/
    bool stifle();
    
    /**
     * Free wheel
     **/
    bool freeWheel();
    
    /**
     * Set speed
     **/
    bool setSpeed(unsigned short pSpeed);
    
protected :
    unsigned short port1;
    unsigned short port2;
    unsigned short port3;
    unsigned short speed;
    
};
#endif /* NHOMotor_hpp */
