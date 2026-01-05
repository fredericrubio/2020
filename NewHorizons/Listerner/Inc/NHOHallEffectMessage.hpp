#ifndef IMP_ImageSizeMessageBody_hpp
#define IMP_ImageSizeMessageBody_hpp

#include "NHOMessage.hpp"
#include "NHOHallEffectData.hpp"

class NHOHallEffectMessage : public NHOMessage {
    
    public :
    /**
     * Constructors
     **/
    NHOHallEffectMessage(long long pDate );
    NHOHallEffectMessage(const NHOHallEffectMessage* pMsg);
    NHOHallEffectMessage(const NHOHallEffectMessage& pMsg);
    /**
     * Destructor
     **/
    virtual ~NHOHallEffectMessage();
    
    virtual bool serialize();
    virtual bool unserialize();

    virtual unsigned int computeSize();
    
    /**
     * Getters and setters
     **/
    inline void setData(const int pSize, const char* pMsg) {
        if (data != NULL) {
            free(data);
            data = NULL;
        }
        data = (char *) calloc(pSize, sizeof(char));
        memcpy(data, pMsg, pSize * sizeof(char));
    };
    
    inline void setHallEffectData(NHOHallEffectData* const pData) {
        if (this->HEData != NULL) {
            delete this->HEData;
        }
        // NHOFILE_LOG(logDEBUG) << "NHOHallEffectMessage::setHallEffectData - post delete.\n";
        this->HEData = pData;
    };
    
    inline NHOHallEffectData* const getHEData() const {return this->HEData;};
    
    protected :
    NHOHallEffectData* HEData;

};
#endif 
