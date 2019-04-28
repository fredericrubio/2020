//
//  NHOSensorData.hpp
//  Sensor
//
//  Created by Frédéric Rubio on 09/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOSensorData_hpp
#define NHOSensorData_hpp

#include "NHOData.hpp"

class NHODataSerializer;

class NHOSensorData : public NHOData {
protected:
    /**
     *
     **/
    char* data;
    
    unsigned short size;
    
    long long date;
    
public:

//    virtual bool serialize(const NHODataSerializer* pSerializer) const = 0;
    
    inline virtual void setDate(const long long pDate) {date = pDate;};
    inline virtual const long long getDate() const {return date;};
    
};
#endif /* NHOSensorData_hpp */
