//
//  NHODataSerializer.hpp
//  Network
//
//  Created by Frédéric Rubio on 22/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHODataSerializer_hpp
#define NHODataSerializer_hpp

class NHOCameraData;

/**
 *
 **/
class NHODataSerializer {
    
public:
    
    NHODataSerializer();
    
    bool serialize(const NHOCameraData* pData) const;
    
protected:
    
};
#endif /* NHOMessageFactory_hpp */
