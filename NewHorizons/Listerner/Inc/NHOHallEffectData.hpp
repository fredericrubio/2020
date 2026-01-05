#ifndef NHOHallEffectData_H
#define NHOHallEffectData_H

#include <string.h>

#include "NHOSensorData.hpp"

class NHOHallEffectData : public NHOSensorData {
    
public:    
    NHOHallEffectData(const int pinNumber);
    NHOHallEffectData(const int pinNumber, const long long pDate);
    /**
     * Copy constructor.
     **/
    NHOHallEffectData(const NHOHallEffectData& orig);
    NHOHallEffectData(const NHOHallEffectData* orig);
    virtual ~NHOHallEffectData();
    
    /**
     * Returns the size (in bytes) of a HEM message
     **/
    static inline size_t getSize() {
        return (sizeof(date) + sizeof(hallEffect)) ;
    }
    
    /**
     * Getters
     **/
    inline short getHallEffect() const {return hallEffect;};

    /**
     * Setters
     **/
    inline void setHallEffect(const short pHallEffect) {hallEffect = pHallEffect;};
    inline void setDate(long long pDate) {this->date = pDate;};
    inline const int getPinNumber() const  {return this->valuePin;}

    /**
     * Update health monitoring sata
     **/
    bool fetch();
    
private: 
    short hallEffect;
    uint8_t valuePin;

    /**
     * Fetch Hall Effect
     **/
    bool fetchHallEffect();
    
};

#endif /* NHOHallEffectData_H */

