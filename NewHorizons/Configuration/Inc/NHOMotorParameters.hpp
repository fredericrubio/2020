/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   NHOMotorParameters.h
 * Author: fredericrubio
 *
 * Created on 21 août 2019, 11:48
 */

#ifndef NHOMOTORPARAMETERS_H
#define NHOMOTORPARAMETERS_H

#include <array>

class NHOMotorParameters {
public:
    typedef enum {MOTOR_EN = 0, MOTOR_A, MOTOR_B} NHOMotorPort;
    typedef enum {RIGHT = 0, LEFT, UNKNOWN} NHOMotorType;

    NHOMotorParameters();
    NHOMotorParameters(const NHOMotorParameters& orig);
    virtual ~NHOMotorParameters();
    
    inline void addPort(const NHOMotorPort pIndex, const unsigned short pPort) {ports[pIndex]=pPort;};
    inline void setType(const NHOMotorType pType) {
        type = pType;
    }
    
protected:
        std::array<unsigned short, 3> ports;
        NHOMotorType type;
        
private:

};

#endif /* NHOMOTORPARAMETERS_H */

