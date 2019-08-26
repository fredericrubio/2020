//
//  NHODEXM.hpp
//  Rover
//
//  Created by Frédéric Rubio on 03/06/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHODEXM_hpp
#define NHODEXM_hpp

#include "NHOTCMessage.hpp"
#include "NHOTemplateFullDuplexConnectedReceiver.hpp"
#include "NHOMotor.hpp"
#include <thread>

class NHOTimer;

class NHODEXM: public NHOObserver<unsigned int> {
    
public:
    
    /**
     * Constructor
     **/
    NHODEXM();
    
    /**
     * Destructor
     **/
    ~NHODEXM();
    
    /**
     * Configure.
     **/
    bool configure() ;
    
    /**
     * Stop anything in progress.
     **/
    bool stop() ;
    
    /**
     * Execute the current TC.
     **/
    bool execute( NHOTCMessage* );

    /**
     * Return true if a TC execution is in progress.
     **/
    bool executionInProgress();
    
    /**
     *
     **/
    virtual void refresh(unsigned int* const parameter);

protected:
    
    // motors
    NHOMotor*   motorR;
    NHOMotor*   motorL;
    bool        motion;
    
    // timer
    NHOTimer* timer;

    // TC to execute
    NHOTCMessage*   TC;
    
    /**
     * WHen time is up, stop any TC in progress
     **/
    bool startTimer(const int pMilliSecs);
    
    /**
     * Consedring the current context, validate (or not) the
     * parameter TC.
     * An obstacle blocking the rover could be a raeson to not validate a TC.
     * A TC must be
     **/
    bool validate();
    
    /**
     * Execute the current TC.
     **/
    bool execute();
    
    bool forward();
    bool reverse();
    bool stopMotion();
    bool turnLeft();
    bool turnRight();

    bool checkMotors();

};
#endif /* NHODEXM_hpp */
