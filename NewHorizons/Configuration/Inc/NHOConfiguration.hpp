//
//  NHOConfiguration.hpp
//  Configuration
//
//  Created by Frédéric Rubio on 16/06/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOConfiguration_hpp
#define NHOConfiguration_hpp

#include "NHOCCParameters.hpp"
#include "NHOCameraParameters.hpp"
#include "NHORoverParameters.hpp"
#include "NHOSensorParameters.hpp"

#include "tinyxml2.h"

using namespace tinyxml2;

static const char* CAMERA_ROOT = "camera";
static const char* HEM_ROOT = "hem";
static const char* CC_ROOT = "cc";
static const char* ROVER_ROOT = "rover";
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
static const char* MOTOR_A = "motorA";
static const char* MOTOR_B = "motorB";
static const char* MOTOR_FAST_STOP = "MOTOR_EN";
static const char* MOTOR_FORWARD = "MOTOR_A";
static const char* MOTOR_REVERSE = "MOTOR_B";
static const char* MOTOR_TYPE = "type";
static const char* MOTOR_RIGHT = "right";
static const char* MOTOR_LEFT = "left";


class NHOConfiguration {
    
public:
    
    /**
     * Set the configuration file nme
     **/
    static bool initialize(const std::string&);
    
    /**
     * Returns the camera configuration (NULL in case of error).
     **/
    static NHOCameraParameters* getCameraConfiguration();
    
    /**
     * Returns the HEM configuration (NULL in case of error).
     **/
    static NHOSensorParameters* getHEMConfiguration();
    
    /**
     * Returns the CC configuration (NULL in case of error).
     **/
    static NHOCCParameters* getCCConfiguration();
    
    /**
     * Returns the Rover configuration (NULL in case of error).
     **/
    static NHORoverParameters* getRoverConfiguration();
    
protected:
    std::string fileName;
    XMLDocument document;

    /**
     * Load the configuration file.
     **/
    bool load();
    
    /**
     *
     **/
    static NHOMotorParameters* getMotorConfiguration(const XMLElement* const) ;

    
};

#endif /* NHOConfiguration_hpp */
