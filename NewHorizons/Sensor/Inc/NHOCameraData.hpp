//
//  NHOCameraData.hpp
//  Sensor
//
//  Created by Frédéric Rubio on 10/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOCameraData_hpp
#define NHOCameraData_hpp

#include <stdlib.h>

#include "NHOSensorData.hpp"

class NHOImage;

class NHOCameraData: public NHOSensorData {

public:
    /**
     Default constructor
     **/
    NHOCameraData(const unsigned short pCameraId);
    /**
     * Copy constructor
     **/
    NHOCameraData(const NHOCameraData & pCameraData);
    
    /**
     Default destructor
     **/
    virtual ~NHOCameraData();
    
    /**
     *
     */
    inline void setImage(NHOImage* const pImage) {image = pImage;};
    
    /**
     *
     **/
    inline NHOImage* getImage() const {return image;};
    
    inline const unsigned short getId() const { return cameraId ;};
    
private :

    NHOImage*   image;
    unsigned short cameraId;

};
#endif /* NHOCameraData_hpp */
