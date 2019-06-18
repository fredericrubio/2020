//
//  NHOSubject.hpp
//  Utils
//
//  Created by Frédéric Rubio on 04/06/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOSubject_hpp
#define NHOSubject_hpp

#include <list>
#include <mutex>

#include "NHOObserver.hpp"
#include "NHOLOG.hpp"

template <class T>
class NHOSubject {

public:
    
    /**
     * Constructor.
     **/
    NHOSubject() {
        
        value = NULL;
        observers.clear();
        
    }
    
    /**
     * Attach a new observer.
     **/
    void attach(NHOObserver<T>* obs) {
        observers.push_back(obs);
    };
    
    /**
     * Notify a change.
     **/
    void notify() {
        NHOFILE_LOG(logDEBUG) << "NHOSubject::notify <"<< observers.size()<<">\n";
        mutex.lock();
        for (NHOObserver<T>* observer : observers) {
            observer->refresh(value);
        }
        mutex.unlock();
    };
    
    /**
     * Set the new value.
     **/
    void setVal(const T* pVal) {
        mutex.lock();
        value = new T(*pVal);
        mutex.unlock();
    };
    
    /**
     * Return the current value.
     **/
    const T getVal() const {
        return value;
    };
    
protected:
    std::list<NHOObserver<T>*> observers;
    T* value;
    std::mutex mutex;

};

#endif /* NHOSubject_hpp */
