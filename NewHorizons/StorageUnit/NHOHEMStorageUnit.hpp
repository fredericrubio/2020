//
//  NHOHEMStorageUnit.hpp
//  StorageUnit
//
//  Created by Frédéric Rubio on 28/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOHEMStorageUnit_hpp
#define NHOHEMStorageUnit_hpp

#include <string>
#include <thread>
#include <stdio.h>

class NHOHEMData;

class NHOHEMStorageUnit {
    
    public :
    NHOHEMStorageUnit(const std::string pHostName, const int pDataPort);
    
    bool initiate();
    
    bool terminate();
    
    bool receiveHEM();
    
protected:
    std::string hostName;
    
    unsigned int dataPort;
    int dataSocket;
    std::thread* dataThread;
    
    // HEM date
    NHOHEMData* data;

};

#endif /* NHOHEMStorageUnit_hpp */
