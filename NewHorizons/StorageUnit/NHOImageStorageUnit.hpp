//
//  NHOImageStorageUnit.hpp
//  StorageUnit
//
//  Created by Frédéric Rubio on 28/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOImageStorageUnit_hpp
#define NHOImageStorageUnit_hpp

#include <string>
#include <thread>
#include <stdio.h>

#include "NHOCameraData.hpp"
#include "NHOImage.hpp"

class NHOImageStorageUnit {
    
    public :
    NHOImageStorageUnit(const std::string pHostName, const int pDataPort);
    
    bool initiate();
    
    bool terminate();
    
    bool waitForConnection();
    
    bool receiveInfoMessage();
    
    bool receiveImageMessage();
    
    bool sendMessage(const size_t pSize, const char* const pMessage);
    
    inline void setImageSize(const unsigned short pWidth, const unsigned short pHeight) {
        cmaeraData.getImage()->setWidth(pWidth);
        cmaeraData.getImage()->setHeight(pHeight);
    }
    
    inline void setImageFormat(const NHOImage::IMAGE_FORMAT pFormat) {
        cmaeraData.getImage()->setFormat(pFormat);
    }
protected:
    unsigned int dataPort;
    
    std::string hostName;
    
    int dataSocket;
    
    std::thread* dataThread;
    std::thread* connectionThread;
    
    // Image parameters
    unsigned int imageWidth;
    unsigned int imageHeight;
    NHOImage::IMAGE_FORMAT imageFormat;
    NHOCameraData   cmaeraData;
};

#endif /* NHOImageStorageUnit_hpp */
