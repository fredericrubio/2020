//
//  NHOCommandCenter.hpp
//  CommandCenter
//
//  Created by Frédéric Rubio on 01/06/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOCommandCenter_hpp
#define NHOCommandCenter_hpp

#include <mutex>

#include "NHOTCMessage.hpp"
#include "NHOTemplateFullDuplexConnectedEmitter.hpp"
#include "NHOTemplateBroadcastReceiver.hpp"
#include "NHOHEMMessage.hpp"

class NHOCommandCenter : NHOObserver<NHOHEMMessage> {
    
public:
    
    /**
     *
     **/
    static bool initialize();

    /**
     * Send a 'Forward' TC
     **/
    static bool sendForwardTC();

    /**
     * Send a 'Reverse' TC
     **/
    static bool sendReverseTC();
    
    /**
     * Send a 'Stop' TC
     **/
    static bool sendStopTC();
    
    /**
     * Send a 'Turn Left' TC
     **/
    static bool sendTurnLeftTC();
    
    /**
     * Send a 'Turn right' TC
     **/
    static bool sendTurnRightTC();
    
    /**
     * Terminate properly and safely
     **/
    static bool terminate();
    
    /**
     * Get the singleton.
     **/
    static NHOCommandCenter* const get();
    
    /**
     * Get the latest HEM message.
     **/
    virtual void refresh(NHOHEMMessage* const parameter) ;

    /**
     *
     **/
    inline static const NHOHEMMessage* getHEM() {
NHOCommandCenter::get()->mutex.lock();
        NHOHEMMessage* result = NULL;
        if (NHOCommandCenter::get()->hem != NULL) {
            result = new NHOHEMMessage(*NHOCommandCenter::get()->hem);
        }
        else {
            result = new NHOHEMMessage(clock());
        }
NHOCommandCenter::get()->mutex.unlock();
        return result;
    }
    
protected:
    std::mutex mutex;
    NHOHEMMessage*  hem;
    NHOTemplateFullDuplexConnectedEmitter<NHOTCMessage>* emitter;
    NHOTemplateBroadcastReceiver<NHOHEMMessage>* HEMReceiver;

    /**
     * Constructor.
     **/
    NHOCommandCenter();
    /**
     * Destructor.
     **/
    ~NHOCommandCenter();

    inline NHOTemplateFullDuplexConnectedEmitter<NHOTCMessage>* const getEmitter()  const {
        return emitter;
    }
    inline NHOTemplateBroadcastReceiver<NHOHEMMessage>* const getHEMReceiver()  const {
        return HEMReceiver;
    }

    /**
     * Terminate properly and safely
     **/
    bool finish();

};
#endif /* NHOCommandCenter_hpp */
