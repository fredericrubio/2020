//
//  NHOConfiguration.hpp
//  Configuration
//
//  Created by Frédéric Rubio on 16/06/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOConfiguration_hpp
#define NHOConfiguration_hpp

#include <string>
#include "NHOLOG.hpp"
#include "tinyxml2.h"
#include "NHOCameraParameters.hpp"

using namespace tinyxml2;

static const char* CAMERA_ROOT = "camera";
static const char* HEM_ROOT = "hem";
static const char* CC_ROOT = "cc";
static const char* IMAGE_SERVER = "img_server";
static const char* SAMPLING = "sampling";
static const char* VALUE = "value";
static const char* PERIOD = "period";
static const char* EMISSION = "emission";
static const char* STORAGE = "storage";
static const char* HOST_NAME = "hostname";
static const char* PORT = "port";
static const char* PORT1 = "port1";
static const char* PORT2 = "port2";

class NHOCCParameters;

class NHOConfiguration {
    
public:
    
    /**
     * Set the configuration file nme
     **/
    static bool initialize(const std::string&);
    
    /**
     * Returns the camera configuration (NULL in case of error).
     **/
    static NHOCameraParameters* getCemaraConfiguration();
    
    /**
     * Returns the HEM configuration (NULL in case of erreor.
     **/
    static NHOSensorParameters* getHEMConfiguration();
    
    /**
     * Returns the CC configuration (NULL in case of erreor.
     **/
    static NHOCCParameters* getCCConfiguration();
    
protected:
    std::string fileName;
    XMLDocument document;

    /**
     * Load the configuration file.
     **/
    bool load();
    
};

#endif /* NHOConfiguration_hpp */
