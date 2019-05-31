/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HEM_Data.cpp
 * Author: fredericrubio
 * 
 * Created on 14 octobre 2018, 21:52
 */
#ifdef _RASPBIAN
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include "sys/sysinfo.h"
#endif
#include <stdio.h>
#include "NHOMessageFactory.hpp"
#include "NHOHEMData.hpp"

NHOHEMData::NHOHEMData() {
    
    type = NHOMessageFactory::eHEM;
    
}

NHOHEMData::NHOHEMData(const long long pDate):date(pDate) {
    
    type = NHOMessageFactory::eHEM;
    
}

    
NHOHEMData::NHOHEMData(const NHOHEMData& orig) {
    
    type = NHOMessageFactory::eHEM;
    
}

NHOHEMData::~NHOHEMData() {
}

/**
 * Update health monitoring sata
 **/
bool NHOHEMData::fetch() {
    
    bool lReturn = false;
    
    lReturn = fetchCPUUsage() && fetchMemoryUsage() && fetchTemperature();
    
    return lReturn;
}
    
/**
 * Fetch CPU
 **/
bool NHOHEMData::fetchCPUUsage() {
#ifdef _RASPBIAN
    float load = 0.0;
    int FileHandler;
    char FileBuffer[1024];
    
    // open the file containing the information we're looking for
    FileHandler = open("/proc/loadavg", O_RDONLY);
    if(FileHandler < 0) {
        return false; 
    }
    // read data
    read(FileHandler, FileBuffer, sizeof(FileBuffer) - 1);
    // extract data
    sscanf(FileBuffer, "%f", &load);
    // close file
    close(FileHandler);
    
    cpu = (short) (load * 100.0);
    // manage HERE the 100% limit
    
    return true;
#else
    cpu = (short) (50.0);
    
    return true;
#endif
}

/**
 * Fetch Temperature
 **/
bool NHOHEMData::fetchTemperature() {
#ifdef _RASPBIAN
    double lTemperature = 0.0;
    FILE *lTemperatureFile;
    
    // open data file
    lTemperatureFile = fopen ("/sys/class/thermal/thermal_zone0/temp", "r");
    if (lTemperatureFile == NULL) {
        return false;
    }
    // extract data
    fscanf (lTemperatureFile, "%lf", &lTemperature);
    // close file
    fclose (lTemperatureFile);
    
    // conversion
    temp = (short) lTemperature / 1000;
    
    return true;
#else
    temp = (short) 50;
    
    return true;
#endif    
}

/**
 * Fetch Memory
 **/
bool NHOHEMData::fetchMemoryUsage() {
#ifdef _RASPBIAN
    struct sysinfo memInfo;

    sysinfo (&memInfo);
    long long totalVirtualMem = memInfo.totalram;
    
    //Add other values in next statement to avoid int overflow on right hand side...
    totalVirtualMem += memInfo.totalswap;
    totalVirtualMem *= memInfo.mem_unit;
    
    long long physMemUsed = memInfo.totalram - memInfo.freeram;
    
    //Multiply in next statement to avoid int overflow on right hand side...
    physMemUsed *= memInfo.mem_unit;
    
    long long totalPhysMem = memInfo.totalram;
    //Multiply in next statement to avoid int overflow on right hand side...
    totalPhysMem *= memInfo.mem_unit;

    // convert into mega-bytes
    if (memInfo.mem_unit == 1) {
        physMemUsed /= 1024 ;
        totalPhysMem /= 1024 ;
    }
    
    usedMemory = (short) (((double) physMemUsed / (double) totalPhysMem) * 100.0);
    return true;
    
#else
    usedMemory = (short) 50.0;
    
    return true;
#endif    
}
