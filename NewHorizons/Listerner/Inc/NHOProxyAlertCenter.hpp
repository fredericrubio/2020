//
//  NHOCommandCenter.hpp
//  CommandCenter
//
//  Created by Frédéric Rubio on 01/06/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOProxyAlertCenter_hpp
#define NHOProxyAlertCenter_hpp

#include <mutex>

#include "NHOTCMessage.hpp"
#include "NHOTemplateFullDuplexConnectedEmitter.hpp"
#include "NHOTemplateBroadcastReceiver.hpp"
#include "NHOHallEffectMessage.hpp"

class NHOProxyAlertCenter : NHOObserver<NHOHallEffectMessage> {
    
public:
    
    /**
     *
     **/
    static bool initialize();

    /**
     * Terminate properly and safely
     **/
    static bool terminate();
    
    /**
     * Get the singleton.
     **/
    static NHOProxyAlertCenter* const get();
    
    /**
     * Get the latest HEM message.
     **/
    virtual void refresh(NHOHallEffectMessage* const parameter) ;

    /**
     *
     **/
    inline static NHOHallEffectMessage* const getMessage() {
NHOProxyAlertCenter::get()->mutex.lock();
        NHOHallEffectMessage* result = NULL;
        if (NHOProxyAlertCenter::get()->msg != NULL) {
            result = new NHOHallEffectMessage(NHOProxyAlertCenter::get()->msg);
        }
        else {
            result = new NHOHallEffectMessage(clock());
        }
NHOProxyAlertCenter::get()->mutex.unlock();
        return result;
    }
    
protected:
    std::mutex mutex;
    NHOHallEffectMessage*  msg;
    NHOTemplateBroadcastReceiver<NHOHallEffectMessage>* msgReceiver;

    /**
     * Constructor.
     **/
    NHOProxyAlertCenter();
    /**
     * Destructor.
     **/
    ~NHOProxyAlertCenter();

    inline NHOTemplateBroadcastReceiver<NHOHallEffectMessage>* const getMsgReceiver()  const {
        return msgReceiver;
    }

    /**
     * Terminate properly and safely
     **/
    bool finish();
    
};
#endif /* NHOProxyAlertCenter_hpp */
