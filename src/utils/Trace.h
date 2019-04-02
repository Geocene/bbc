//  Copyright (c) 2019 Bob Brown. All rights reserved.
//

#pragma once

#include "BBCAssert.h"
#include "Singleton.h"

class Trace : public Singleton<Trace>
{
public:
    
    bool initialize(const std::string& iTraceConfigFile)
    {
        if (initalized_)
            return true;
        
        initalized_ = true;
        
        return true;
    }
    
private:
    
    bool initalized_{false};
};
