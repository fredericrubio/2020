//
//  NHOImage.hpp
//  Utils
//
//  Created by Frédéric Rubio on 05/05/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOImage_hpp
#define NHOImage_hpp

class NHOImage {
public:
    /**
     Default constructor
     **/
    NHOImage();
    
    /**
     Copy constructor
     **/
    NHOImage(const NHOImage&);
    
    /**
     * Destructor
     **/
    ~NHOImage();
    
    /*
     *Image formats
     */
    typedef enum {
        FORMAT_YUV420 = 0,
        FORMAT_GRAY,
        FORMAT_BGR,
        FORMAT_RGB,
        FORMAT_IGNORE //do not use
    } IMAGE_FORMAT;
    
    /**
     * Getters and setters
     **/
    inline unsigned int getWidth() const {return width;} ;
    inline unsigned int getHeight() const {return height;} ;
    inline IMAGE_FORMAT getFormat() const {return format;} ;
    inline const unsigned char* getPixels() const {return pixels;} ;
    inline  unsigned int getDataSize() const {return size; } ;
    
    inline void setWidth(const unsigned int pValue) {width = pValue;} ;
    inline void setHeight(const unsigned int pValue) {height = pValue;} ;
    // TO DO: compute the size of the image by taking into account the fromat
    inline void setFormat(const IMAGE_FORMAT pValue) {format = pValue; size = width * height * 3;};
    // To do: take into account the image format
    inline void setPixels(const unsigned int pSize, unsigned char* const pPixels) {
        size = pSize;
        /*           if (pixels != NULL) {
         free(pixels);
         }
         */
        pixels = pPixels;
    } ;
    /*
     *
     */
    void deletePixels() {
        if (pixels != NULL) {
            free(pixels);
            pixels = NULL;
        }
    }
    
    /**
     * Unserialize the sensor data received.
     **/
    //    virtual bool unserialize();
    
    /**
     * Lad an image from disk (debug purpose)
     **/
    bool loadFromDisk(const char* pFileName);
    
    /**
     * Load an image from disk (debug purpose)
     **/
    bool readPPM(const char *filename);
    
    /**
     * Save the image to disk (debug purpose)
     **/
    bool saveToDisk();
    
    /**
     * Serialize the sensor data in order to be sent.
     **/
    /*    bool serialize(const NHODataSerializer* pSerializer) const ;
     */
    private :
    unsigned int width;
    unsigned int height;
    IMAGE_FORMAT format;
    unsigned int size;
    unsigned char* pixels;
    
};
#endif /* NHOImage_hpp */
