//
//  NHORover.hpp
//  Rover
//
//  Created by Frédéric Rubio on 12/05/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHORover_hpp
#define NHORover_hpp

class NHOCamera;
class NHOCameraParameters;
class NHOSensorParameters;
class NHORoverHEM;
class NHODEXM;

#include "NHOObserver.hpp"
#include "NHOTCMessage.hpp"
#include "NHOTemplateFullDuplexConnectedReceiver.hpp"

class NHORover: public NHOObserver<NHOTCMessage> {
    
public:
    /**
     * Constructor
     **/
    NHORover();
    
    /**
     * Destructor
     **/
    ~NHORover();
    
    /**
     *
     **/
    bool initialize();
    
    /**
     *
     **/
    bool start();
    
    /**
     * Observing an TNHOTCMessage emitter.
     **/
    virtual void refresh(NHOTCMessage* const parameter);
    
protected:
    
    NHOTemplateFullDuplexConnectedReceiver<NHOTCMessage>* receiver;

    NHOCamera*  camera;
    NHORoverHEM*  hem;
    NHOCameraParameters* cameraParameters;
    NHOSensorParameters* hemParameters;

    /**
     *
     **/
    bool readConfiguration();
    
    NHODEXM*    dex;
    
};
#endif /* NHORover_hpp */
