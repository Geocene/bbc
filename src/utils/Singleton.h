//  Copyright (c) 2019 Bob Brown. All rights reserved.
//

#pragma once

/**
 * \brief Templated Singleton class
 *
 * Note - There is not destructor as this object exists
 *        the entire time of the program.
 *        All resources will be released when the program is terminated.
 *        Any implementation of a Singleton should not hold resources
 *        such as open files, ports, or other items that would be
 *        cleaned up at destruction time.
 */
template <class _class_>
class Singleton
{
public:
    
    /**
     * Returns a reference to the singleton.
     * This is a static function.
     *
     * Example use case:
     *
     *      class SingletonTest1 : public Singleton<SingletonTest1> {};
     *      SingletonTest1::instance();
     *
     * @return Returns a reference to the singleton object
     */
    static _class_& instance()
    {
        if (instance_ == nullptr)
        {
            instance_ = std::unique_ptr<_class_>(new _class_);
        }
        
        return *instance_;
    }
    
private:
    
    /// The Singleton pointer for the class.
    static std::unique_ptr<_class_> instance_;
};

/// Defining the singleton static instance and setting it to nullptr
/// This is done in the header as it is a templated type.
template <class _class_>
std::unique_ptr<_class_> Singleton<_class_>::instance_ = nullptr;
