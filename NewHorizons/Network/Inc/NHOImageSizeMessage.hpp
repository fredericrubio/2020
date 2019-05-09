//
//  NHOImageSizeMessageBody.hpp
//  ComLib
//
//  Created by Frédéric Rubio on 04/09/2018.
//  Copyright © 2018 Frédéric Rubio. All rights reserved.
//

#ifndef IMP_ImageSizeMessageBody_hpp
#define IMP_ImageSizeMessageBody_hpp

#include "NHOMessage.hpp"
#include "NHOImage.hpp"

class NHOImageSizeMessage : public NHOMessage {
    
    public :
    /**
     * Constructors
     **/
    NHOImageSizeMessage(long long pDate );
    NHOImageSizeMessage(   long long pDate, const unsigned int pWidth,
                                const unsigned int pHeight, 
                                const NHOImage::IMAGE_FORMAT pFormat,
                                const unsigned int pDataSize) ;
    /**
     * Destructor
     **/
    virtual ~NHOImageSizeMessage();
    
    virtual bool serialize();
    virtual bool unserialize();

    virtual unsigned int computeSize();
    
    /**
     * Getters and setters
     **/
    inline unsigned int getWidth() const {return width;} ;
    inline unsigned int getHeight() const {return height;} ;
    inline const NHOImage::IMAGE_FORMAT getFormat() const {return format;} ;
    inline unsigned int getDataSize() const {return dataSize;} ;
    
    inline void setWidth(const unsigned int pValue) {width = pValue;} ;
    inline void setHeight(const unsigned int pValue) {height = pValue;} ;
    inline void setFormat(const NHOImage::IMAGE_FORMAT pValue) {format = pValue;} ;
    inline void setDataSize(const unsigned int pValue) {dataSize = pValue;} ;

    protected :
    unsigned int width;
    unsigned int height;
    NHOImage::IMAGE_FORMAT format;
    unsigned int dataSize;

};
#endif /* IMP_ImageSizeMessageBody_hpp */
