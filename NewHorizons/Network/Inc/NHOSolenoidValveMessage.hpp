#ifndef IMP_SolenoidValveMessageBody_hpp
#define IMP_SolenoidValveMessageBody_hpp

#include "NHOMessage.hpp"
#include "NHOSolenoidValveData.hpp"
#include "NHOLOG.hpp"

class NHOSolenoidValveMessage : public NHOMessage {
    
    public :
    /**
     * Constructors
     **/
    NHOSolenoidValveMessage(long long pDate);
    NHOSolenoidValveMessage(const NHOSolenoidValveMessage*);

    /**
     * Destructor
     **/
    virtual ~NHOSolenoidValveMessage();
    
    virtual bool serialize();
    virtual bool unserialize();

    virtual unsigned int computeSize();
    
    /**
     * Getters and setters
     **/
    inline void setData(const int pSize, const char* pMsg) {
        // NHOFILE_LOG(logDEBUG) << "\nNHOSolenoidValveMessage::setData.\n";
        this->size = pSize;
        this->data = (char *) pMsg;
        return;

        if (this->data != NULL) {
#ifdef ESP32_ADAFRUIT_FEATHER     
            heap_caps_free(this->data);
#else
            delete this->data;
#endif            
            this->data = NULL;
        }
        NHOFILE_LOG(logDEBUG) << "NHOSolenoidValveMessage::setData - 2.\n";
#ifdef ESP32_ADAFRUIT_FEATHER     
        this->data = (char *) heap_caps_malloc(pSize, MALLOC_CAP_SPIRAM);
        // this->data = (char *) heap_caps_malloc(this->getSize() * sizeof(char), MALLOC_CAP_SPIRAM);
#else
        this->data = new char[pSize];
#endif
        NHOFILE_LOG(logDEBUG) << "NHOSolenoidValveMessage::setData - 3 <" << pSize * sizeof(char) << ">\n";
        try {
            memcpy((void *) this->data, (void *) pMsg, pSize * sizeof(char));
        }
        catch(std::exception exception) {
            NHOFILE_LOG(logDEBUG) << "NHOSolenoidValveMessage::setData " << exception.what() << "\n";
        }
        NHOFILE_LOG(logDEBUG) << "NHOSolenoidValveMessage::setData - 4.\n";
    };
    
    /**
     * Set the payload.
     **/
    inline void setSolenoidValveData(NHOSolenoidValveData* const pData) {
        if (this->solenoidValveData != NULL) {
#ifdef ESP32_ADAFRUIT_FEATHER     
            heap_caps_free(this->soleniodValveData);
#else
            delete this->solenoidValveData;
#endif
        }
        // NHOFILE_LOG(logDEBUG) << "NHOSolenoidValveMessage::setHallEffectData - post delete.\n";
        this->solenoidValveData = pData;
    };

    /**
     * Returns Solenoid Valve Data
     */
    inline NHOSolenoidValveData* const getSoleniodValveData() const {return this->solenoidValveData;};

    protected :
        NHOSolenoidValveData*   solenoidValveData;
};
#endif 
