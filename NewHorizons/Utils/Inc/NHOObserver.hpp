//
//  NHOObserver.hpp
//  Utils
//
//  Created by Frédéric Rubio on 04/06/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOObserver_hpp
#define NHOObserver_hpp

template <class T>
class NHOObserver {

public:
    
    virtual void refresh(T* const parameter) = 0;
    
};

#endif /* NHOObserver_hpp */
