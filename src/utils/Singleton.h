//  Copyright (c) 2019 Bob Brown. All rights reserved.
//

#pragma once

template <class _class_>
class Singleton
{
public:
    static _class_& instance()
    {
        if (instance_ == nullptr)
        {
            instance_ = std::unique_ptr<_class_>(new _class_);
        }
        
        return *instance_;
        //return *instance_.get();
    }
    
private:
    static std::unique_ptr<_class_> instance_;
};

template <class _class_>
std::unique_ptr<_class_> Singleton<_class_>::instance_ = nullptr;
