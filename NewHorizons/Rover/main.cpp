//
//  main.cpp
//  Rover
//
//  Created by Frédéric Rubio on 30/03/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include <unistd.h>

#include "NHOLOG.hpp"
#ifdef _RASPBIAN
    #include "wiringPi.h"
#endif
#include "NHOCamera.hpp"

#include "NHOCameraData.hpp"

#include "NHORover.hpp"
#include "tinyxml2.h"
#include "NHOConfiguration.hpp"
#include "NHOCCParameters.hpp"

int main(int argc, const char * argv[]) {
    
    std::string lConfigurationFileName("/Users/fredericrubio/Development/Project/New\ Horizons/Development/NewHorizons/Resources/rover.xml");
    
    if (NHOConfiguration::initialize(lConfigurationFileName) == false) {
        NHOFILE_LOG(logERROR) << "Unable to load config file \n";
        return 1;
    };
    NHOCameraParameters* lCameraConfig = NHOConfiguration::getCemaraConfiguration();
    
    NHOSensorParameters* lHEMConfig = NHOConfiguration::getHEMConfiguration();
    
    NHOCCParameters* lCCParameters = NHOConfiguration::getCCConfiguration();
    
    NHORoverParameters* lRoverParameters = NHOConfiguration::getRoverConfiguration();

    tinyxml2::XMLDocument doc;
    FILE* file = fopen("/Users/fredericrubio/Development/Project/New\ Horizons/Development/NewHorizons/Resources/rover.xml", "r");
    tinyxml2::XMLError loadOkay = doc.LoadFile("/Users/fredericrubio/Development/Project/New\ Horizons/Development/NewHorizons/Resources/rover.xml");
    if (loadOkay != tinyxml2::XML_SUCCESS)
    {
        NHOFILE_LOG(logERROR) << "Unable to load config file <"<<loadOkay<<">\n";;
        return 1;
    }
    
    const char* hostname = new char[256];
    int value;
    // HEM
    tinyxml2::XMLElement* first = doc.FirstChildElement("HEM");
//    tinyxml2::XMLElement* attributeApproachElement = doc.FirstChildElement("HEM");
    first->QueryIntAttribute( "port", &value );
    
    // CC
    tinyxml2::XMLElement* second = doc.FirstChildElement("CC");
    second->QueryIntAttribute( "port", &value );

    // IMG
    tinyxml2::XMLElement* third = doc.FirstChildElement("IMG");
    third->QueryStringAttribute( "hostname", &hostname );
    third->QueryIntAttribute( "port", &value );
    
    // CAMERA
    tinyxml2::XMLElement* fourth = doc.FirstChildElement("camera");
    tinyxml2::XMLElement* sampling = fourth->FirstChildElement("sampling");
    sampling->QueryIntAttribute( "value", &value );
/*
    // ROVER
    tinyxml2::XMLElement* element = doc.FirstChildElement("rover");
    tinyxml2::XMLElement* attributeApproachElement = doc.FirstChildElement("rover")->FirstChildElement( "motorA" );
    attributeApproachElement->QueryIntAttribute( "MOTOR1_EN", &value );
*/
    NHOFILE_LOG(logDEBUG) << "main: start" << std::endl;
#ifdef _RASPBIAN
    if(wiringPiSetup() == -1){ //when initialize wiring failed,print messageto screen
            NHOFILE_LOG(logERROR) << "Setup wiringPi failed !" << std::endl;
            return 1; 
    }
    pinMode(0, OUTPUT);//Set the pin mode
    pinMode(1, OUTPUT);//Set the pin mode
    pinMode(3, OUTPUT);//Set the pin mode
#endif
    
    NHORover*   lRover;
    lRover = new NHORover();
    if (!lRover->initialize()) {
        NHOFILE_LOG(logERROR) << "Rover initialization failed !" << std::endl;
        return -1;
    }
    
    lRover->start();
    
    unsigned int microseconds = 10000000;
    while(true) {
        usleep(microseconds);
    }

    return 0;
    
}
