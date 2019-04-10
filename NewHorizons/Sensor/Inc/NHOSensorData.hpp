//
//  NHOSensorData.hpp
//  Sensor
//
//  Created by Frédéric Rubio on 09/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOSensorData_hpp
#define NHOSensorData_hpp

public class NHOSensorData {
protected:
    /**
     *
     **/
    char* data;
    
    unsigned short size;
    
    long long date;
    
public:
    /**
     * Serialize the sensor data in order to be sent.
     **/
    virtual bool serialize() = 0;

    /**
     * Unserialize the sensor data received.
     **/
    virtual bool unserialize() = 0;

    inline void setDate(const long long pDate) {date = pDate};
    inline const long long getDate() const {return date;};
    
};
#endif /* NHOSensorData_hpp */
