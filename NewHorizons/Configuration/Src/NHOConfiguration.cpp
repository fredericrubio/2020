//
//  NHOConfiguration.cpp
//  Configuration
//
//  Created by Frédéric Rubio on 16/06/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//
#include "NHOConfiguration.hpp"

#include "NHOLOG.hpp"

static NHOConfiguration* singleton = NULL;

/**
 * Set the configuration file nme
 **/
bool NHOConfiguration::initialize(const std::string& pFileName) {
    
    if (pFileName.empty()) {
        NHOFILE_LOG(logERROR) << "NHOConfiguration::initialize: no file name. \n";
        return false;
    }
    if (singleton == NULL) {
        singleton = new NHOConfiguration();
    }

    singleton->fileName = pFileName;
    return singleton->load();
    
}

/**
 * Load the configuration file.
 **/
bool NHOConfiguration::load() {
    
    XMLError loadingError = document.LoadFile(fileName.c_str());
    if (loadingError != XML_SUCCESS)
    {
        NHOFILE_LOG(logERROR) << "NHOConfiguration::load: unable to load config file <"<<loadingError<<">\n";;
        return false;
    }
    return true;
}

/**
 * Returns the camera configuration (NULL in case of error).
 **/
NHOCameraParameters* NHOConfiguration::getCameraConfiguration() {

    if (singleton == NULL) {
        return NULL;
    }
    
    NHOCameraParameters* lCameraParameters = new NHOCameraParameters();
    
    int lInteger = -1;
    XMLElement* lCameraRoot = singleton->document.FirstChildElement(CAMERA_ROOT);
    XMLElement* lSampling = lCameraRoot->FirstChildElement(SAMPLING);
    lSampling->QueryIntAttribute(VALUE, &lInteger);
    lCameraParameters->setSamplingPeriod(lInteger);
    
    bool lBoolean = false;
    XMLElement* lEmission = lCameraRoot->FirstChildElement(EMISSION);
    lEmission->QueryBoolAttribute(VALUE, &lBoolean);
    lEmission->QueryIntAttribute(PERIOD, &lInteger);
    lCameraParameters->setDataEmission(lBoolean);
    lCameraParameters->setDataEmissionPeriod(lInteger);
    
    XMLElement* lStorage = lCameraRoot->FirstChildElement(STORAGE);
    lStorage->QueryBoolAttribute(VALUE, &lBoolean);
    lStorage->QueryIntAttribute(PERIOD, &lInteger);
    lCameraParameters->setStorage(lBoolean);
    lCameraParameters->setStoragePeriod(lInteger);

    int lInteger2 = -1;
    const char* lHostName = new char[256];
    XMLElement* lImgServer = singleton->document.FirstChildElement(IMAGE_SERVER);
    lImgServer->QueryStringAttribute(HOST_NAME, &lHostName);
    lImgServer->QueryIntAttribute(PORT1, &lInteger);
    lImgServer->QueryIntAttribute(PORT2, &lInteger2);
    lCameraParameters->setDataEmissionPort(lInteger);
    lCameraParameters->setServiceEmissionPort(lInteger2);
    std::string lString(lHostName);
    lCameraParameters->setHostName(lString);
    
    return lCameraParameters;
}

/**
 * Returns the HEM configuration (NULL in case of erreor.
 **/
NHOSensorParameters* NHOConfiguration::getHEMConfiguration() {
  
    if (singleton == NULL) {
        return NULL;
    }
    
    NHOSensorParameters* lHEMParameters = new NHOSensorParameters();

    int lInteger = -1;
    XMLElement* lHEMRoot = singleton->document.FirstChildElement(HEM_ROOT);
    XMLElement* lSampling = lHEMRoot->FirstChildElement(SAMPLING);
    lSampling->QueryIntAttribute(VALUE, &lInteger);
    lHEMParameters->setSamplingPeriod(lInteger);

    bool lBoolean = false;
    XMLElement* lEmission = lHEMRoot->FirstChildElement(EMISSION);
    lEmission->QueryBoolAttribute(VALUE, &lBoolean);
    lEmission->QueryIntAttribute(PERIOD, &lInteger);
    lHEMParameters->setDataEmission(lBoolean);
    lHEMParameters->setDataEmissionPeriod(lInteger);

    XMLElement* lStorage = lHEMRoot->FirstChildElement(STORAGE);
    lStorage->QueryBoolAttribute(VALUE, &lBoolean);
    lStorage->QueryIntAttribute(PERIOD, &lInteger);
    lHEMParameters->setStorage(lBoolean);
    lHEMParameters->setStoragePeriod(lInteger);
 
    XMLElement* lPort = lHEMRoot->FirstChildElement(PORT);
    lPort->QueryIntAttribute(VALUE, &lInteger);
    lHEMParameters->setDataEmissionPort(lInteger);

    return lHEMParameters;
}

/**
 * Returns the CC configuration (NULL in case of erreor.
 **/
NHOCCParameters* NHOConfiguration::getCCConfiguration() {
    
    if (singleton == NULL) {
        return NULL;
    }
    
    NHOCCParameters* lCCParameters = new NHOCCParameters();
    
    const char* lHostName = new char[256];
    int lInteger = -1;
    XMLElement* lCCRoot = singleton->document.FirstChildElement(CC_ROOT);
    lCCRoot->QueryStringAttribute(HOST_NAME, &lHostName);
    lCCRoot->QueryIntAttribute(PORT, &lInteger);

    std::string lString(lHostName);
    lCCParameters->setHostName(lString);
    lCCParameters->setPort(lInteger);
    
    return lCCParameters;
}


/**
 * Returns the Rover configuration (NULL in case of error).
 **/
NHORoverParameters* NHOConfiguration::getRoverConfiguration() {
 
    if (singleton == NULL) {
        return NULL;
    }
    
    NHORoverParameters* lRoverParameters = new NHORoverParameters();

    XMLElement* lRoverRoot = singleton->document.FirstChildElement(ROVER_ROOT);
    // first motor
    XMLElement* lMotor = lRoverRoot->FirstChildElement(MOTOR_A);
    lRoverParameters->addMotor(NHOConfiguration::getMotorConfiguration(lMotor));
    
    // second motor
    lMotor = lRoverRoot->FirstChildElement(MOTOR_B);
    lRoverParameters->addMotor(NHOConfiguration::getMotorConfiguration(lMotor));
    
    return lRoverParameters;

}

/**
 *
 **/
NHOMotorParameters* NHOConfiguration::getMotorConfiguration(const XMLElement* const pMotor) {
    
    int lInteger = -1;
    const char* lType = new char[256];

    NHOMotorParameters* lMotorParam = new NHOMotorParameters();
    
    pMotor->QueryIntAttribute(MOTOR_FAST_STOP, &lInteger);
    lMotorParam->addPort(NHOMotorParameters::MOTOR_EN, lInteger);
    pMotor->QueryIntAttribute(MOTOR_FORWARD, &lInteger);
    lMotorParam->addPort(NHOMotorParameters::MOTOR_A, lInteger);
    pMotor->QueryIntAttribute(MOTOR_REVERSE, &lInteger);
    lMotorParam->addPort(NHOMotorParameters::MOTOR_B, lInteger);
    pMotor->QueryStringAttribute(MOTOR_TYPE, &lType);
    std::string lString(lType);
    if (lString.compare(MOTOR_RIGHT) == 0) {
        lMotorParam->setType(NHOMotorParameters::RIGHT);
    }
    else if (lString.compare(MOTOR_LEFT) == 0) {
        lMotorParam->setType(NHOMotorParameters::LEFT);
    }
    else {
        lMotorParam->setType(NHOMotorParameters::UNKNOWN);
    }

    return lMotorParam;
    
}

