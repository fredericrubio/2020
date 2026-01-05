//
//  NHOMessageDispatcher.hpp
//  Hearing
//
//  Created by Frédéric Rubio on 11/09/2025.
//  Copyright © 2025 Frédéric Rubio. All rights reserved.
//

#ifndef NHOMessageDispatcher_hpp
#define NHOMessageDispatcher_hpp

#include <stdio.h>
#include <list>
#include <mutex>
#ifdef ESP32_ADAFRUIT_FEATHER
    #include "Sensor/NHOSolenoidValveData.hpp"
    #include "Utils/NHOObserver.hpp"
    #include "Network/NHOSolenoidValveMessage.hpp"
#else
    #include "NHOSolenoidValveData.hpp"
    #include "NHOObserver.hpp"
    #include "NHOSolenoidValveMessage.hpp"
#endif

class NHORegistree;

class NHOSolenoidValveMessageDispatcher : public NHOObserver<NHOSolenoidValveMessage> {

public:
    /**
     *
     **/
    class NHORegistree {
    public:
        NHORegistree();
        ~NHORegistree();
        virtual bool process(const NHOSolenoidValveMessage* const pMsg) = 0;
    };
    
    static const NHOSolenoidValveMessageDispatcher* get();
    
    static bool dispatch(const NHOSolenoidValveMessage* const pMsg);
    
    static bool subscribe(const NHOSolenoidValveData::NHOSolenoidValvePayload pMsgType,
                const NHORegistree* const pRegistree);
    
    virtual void refresh(NHOSolenoidValveMessage* const parameter);

protected:

    /**
     * Constructor
     **/
    NHOSolenoidValveMessageDispatcher();
    
    /**
     * Destructor
     **/
    ~NHOSolenoidValveMessageDispatcher();
    
    struct NHORegistreePayload {
        NHOSolenoidValveData::NHOSolenoidValvePayload payload;
        NHORegistree*    registree;
    };
    
    std::list<NHORegistreePayload*> registrees;
    std::mutex mutex;
};
#endif /* NHOMessageDispatcher_hpp */
