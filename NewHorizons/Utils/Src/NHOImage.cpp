//
//  NHOImage.cpp
//  Utils
//
//  Created by Frédéric Rubio on 05/05/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include <iostream>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <sys/ioctl.h>
#include <netinet/tcp.h>
#include <iostream>
#include<stdio.h>
#include<stdlib.h>

#include "NHOImage.hpp"

#define RGB_COMPONENT_COLOR 255

/**
 *
 **/
NHOImage::NHOImage():pixels(NULL){
    
}

/**
 * Save the image to disk (debug purpose)
 **/
bool NHOImage::saveToDisk() {
    
    std::string lFileName = std::to_string(clock()) + "_image.ppm";
    std::ofstream outFile ( lFileName, std::ios::binary );
    outFile<<"P6\n" << getWidth() << " " << getHeight() << " 255\n";
    //     outFile<<"P6\n" << "#Raspery\n" << getWidth() << " " << getHeight() << "\n255\n";
    outFile.write ( ( char* ) getPixels(), getDataSize());
    outFile.close();
    
    return (true);
}

/**
 * Load an image from disk (debug purpose)
 **/
bool NHOImage::readPPM(const char *filename) {
    
    if (pixels != NULL) {
        free(pixels);
    }
    
    char buff[16];
    FILE *fp;
    int c, rgb_comp_color;
    //open PPM file for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "Unable to open file '%s'\n", filename);
        exit(1);
    }
    
    //read image format
    if (!fgets(buff, sizeof(buff), fp)) {
        perror(filename);
        exit(1);
    }
    
    //check the image format
    if (buff[0] != 'P' || buff[1] != '6') {
        fprintf(stderr, "Invalid image format (must be 'P6')\n");
        exit(1);
    }
    
    //check for comments
    c = getc(fp);
    while (c == '#') {
        while (getc(fp) != '\n') ;
        c = getc(fp);
    }
    
    ungetc(c, fp);
    //read image size information
    int lWidth, lHeight;
    if (fscanf(fp, "%d %d", &lWidth, &lHeight) != 2) {
        fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
        exit(1);
    }
    
    //read rgb component
    if (fscanf(fp, "%d", &rgb_comp_color) != 1) {
        fprintf(stderr, "Invalid rgb component (error loading '%s')\n", filename);
        exit(1);
    }
    
    //check rgb component depth
    if (rgb_comp_color!= RGB_COMPONENT_COLOR) {
        fprintf(stderr, "'%s' does not have 8-bits components\n", filename);
        exit(1);
    }
    
    while (fgetc(fp) != '\n') ;
    //memory allocation for pixel data
    unsigned char* lBuffer = (unsigned char *) calloc(lWidth * lHeight * 3, sizeof(unsigned char));
    if (!lBuffer) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }
    
    //read pixel data from file
    if (fread(lBuffer, 3 * lWidth, lHeight, fp) != lHeight) {
        fprintf(stderr, "Error loading image '%s'\n", filename);
        exit(1);
    }
    
    this->setWidth(lWidth);
    this->setHeight(lHeight);
    this->setFormat(FORMAT_RGB);
    this->setPixels(getWidth() * getHeight() * 3, lBuffer);
    
    fclose(fp);
    return true;
}

/**
 * Load an image from disk (debug purpose)
 **/
bool NHOImage::loadFromDisk(const char* pFileName) {
    std::string line;
    std::string lFileName = pFileName;
    std::ifstream inFile ( lFileName ,std::ios::binary );
    std::string delimiter = " ";
    
    // width height 255\n
    std::getline(inFile,line);
    std::getline(inFile,line);
    std::getline(inFile,line);
    
    unsigned long lHeightIndex = line.find(delimiter);
    std::string token = line.substr(0, lHeightIndex);
    int lWidth = std::stoi( token );
    
    line.erase(0, lHeightIndex + delimiter.length());
    token = line.substr(0, lHeightIndex);
    int lHeight = std::stoi( token );
    
    // last useless line
    std::getline(inFile,line);
    
    unsigned char* lBuffer = (unsigned char *) calloc(lWidth * lHeight * 3, sizeof(unsigned char));
    
    inFile >> lBuffer;
    inFile.close();
    
    this->setWidth(lWidth);
    this->setHeight(lHeight);
    this->setFormat(FORMAT_RGB);
    this->setPixels(getWidth() * getHeight() * 3, lBuffer);
    
    return true;
    
}
