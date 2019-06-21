//
//  NHORoverParameters.hpp
//  Configuration
//
//  Created by Frédéric Rubio on 18/06/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHORoverParameters_hpp
#define NHORoverParameters_hpp

#include <list>
#include <array>

class NHORoverParameters {
    
public:
    typedef enum {MOTOR_EN = 0, MOTOR_A, MOTOR_B} NHOMotorPort;
    typedef enum {RIGHT = 0, LEFT, UNKNOWN} NHOMotorType;

    class NHOMotorParameters {
    public:
        inline void addPort(const NHOMotorPort pIndex, const unsigned short pPort) {ports[pIndex]=pPort;};
        inline void setType(const NHOMotorType pType) {
            type = pType;
        }
    protected:
        std::array<unsigned short, 3> ports;
        NHOMotorType type;
    };
    
    /**
     * Constructor
     **/
    NHORoverParameters();
    
    /**
     *
     **/
    inline void addMotor(const NHOMotorParameters* const pMotorParam) {
        motors.push_back(pMotorParam);
    }
protected:
    std::list<const NHOMotorParameters * const> motors;
    
};
#endif /* NHORoverParameters_hpp */
