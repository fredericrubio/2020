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

class NHORover {
    
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
    
protected:
    
    NHOCamera*  camera;
    NHOCameraParameters* cameraParameters;

    /**
     *
     **/
    bool readConfiguration();
    
};
#endif /* NHORover_hpp */
