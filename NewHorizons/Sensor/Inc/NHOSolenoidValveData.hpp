#ifndef NHOSolenoidValveData_H
#define NHOSolenoidValveData_H

#include <string.h>

#include "NHOSensorData.hpp"

class NHOSolenoidValveData : public NHOSensorData {
    
public:
    typedef enum {eUnknown, ePing, ePong, eAckMessage, eNackMessage, eOpen, eClose} NHOSolenoidValvePayload;
    
    NHOSolenoidValveData();
    
    NHOSolenoidValveData(const NHOSolenoidValvePayload pCommand, const long long pDate);
    
    /**
     * Copy constructor.
     **/
    NHOSolenoidValveData(const NHOSolenoidValveData& orig);
    NHOSolenoidValveData(const NHOSolenoidValveData* orig);
    virtual ~NHOSolenoidValveData();
    
    /**
     * Returns the size (in bytes) of a HEM message
     **/
    static inline size_t getSize() {
        return (sizeof(date) + sizeof(command)) ;
    }
    
    inline void setDate(long long pDate) {this->date = pDate;};
    inline void setCommand(const NHOSolenoidValvePayload pCommand) {
        this->command = pCommand;
    };
    
    inline NHOSolenoidValvePayload getCommand() {return this->command;};
    
private:
    NHOSolenoidValveData::NHOSolenoidValvePayload command;
    
    friend class NHOMemory;
};

#endif /* NHOSolenoidValveData_H */

