//
//  NHOData.hpp
//  Sensor
//
//  Created by Frédéric Rubio on 22/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOData_hpp
#define NHOData_hpp

#include "NHOMessageFactory.hpp"

class NHOData {
    
public:
    
    NHOData();
    NHOData(const NHOData&);

    virtual inline NHOMessageFactory::NHOMessageType getType() const {return type;} ;
    
protected :
    
    NHOMessageFactory::NHOMessageType  type;
    
    
};
#endif /* NHOData_hpp */
