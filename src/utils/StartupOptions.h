//  Copyright (c) 2019 Bob Brown. All rights reserved.
//

#pragma once

#include "BBCAssert.h"
#include "Singleton.h"

#ifdef BBC_USE_BOOST
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <set>
#include <exception>
#include <iostream>
using namespace boost::property_tree;
#endif

/// Boost tag for reading attributes
///
#define XMLATTR "<xmlattr>"

#define SO_STARTUPOPTIONS "StartupOptions"
#define SO_DOT "."
#define SO_OPTION "Option"
#define SO_TYPE "type"
#define SO_NAME "name"
#define SO_VALUE "value"

#define SO_TYPENAME_BOOL "StartupOptionBool"
#define SO_TYPENAME_INT32 "StartupOptionInt32"
#define SO_TYPENAME_INT64 "StartupOptionInt64"
#define SO_TYPENAME_FLOAT "StartupOptionFloat"
#define SO_TYPENAME_STRING "StartupOptionString"

class StartupOptions;
class StartupOption;

class IStartupOptions : public Singleton<StartupOptions>
{
    friend class StartupOptionBool;
    friend class StartupOptionInt32;
    friend class StartupOptionInt64;
    friend class StartupOptionFloat;
    friend class StartupOptionString;

public:
    
    /// The interface needs it's own factory such that we can have access to the interface APIs.
    /// The base Singleton doesn't have all of the needed APIs.
    ///
    static IStartupOptions& instance()
    {
        return reinterpret_cast<IStartupOptions&>(Singleton<StartupOptions>::instance());
    }

    virtual bool initialize(const std::string& iStartupOptionsFile) = 0;

    virtual bool writeDefaultStartupOptionsFile(const std::string& iStartupOptionsFile) = 0;
    
protected:
    
    virtual bool add(StartupOption* iOption) = 0;
};

class StartupOption
{
public:
    
    StartupOption() {}

    virtual ~StartupOption() {}

    const std::string& name()
    {
        return name_;
    }

    const std::string& typeName()
    {
        return typeName_;
    }

protected:
    std::string name_;
    std::string typeName_;
};

template<class _class_>
class StartupOptionTyped : public StartupOption
{
public:
    
    StartupOptionTyped() {}
    
    virtual ~StartupOptionTyped() {}
    
    void set(const _class_& iVal)
    {
        value_ = iVal;
    }
    
    const _class_& getDefault()
    {
        return defaultValue_;
    }
    
    operator _class_()
    {
        return value_;
    }
    
protected:
    _class_ value_;
    _class_ defaultValue_;
};

class StartupOptionBool : public StartupOptionTyped<bool>
{
public:
    
    StartupOptionBool(const std::string& iName, bool iVal)
    {
        name_ = iName;
        typeName_ = SO_TYPENAME_BOOL;
        value_ = iVal;
        defaultValue_ = iVal;
        
        IStartupOptions::instance().add(this);
    }
    
    virtual ~StartupOptionBool() {}
    
private:
    
};

class StartupOptionInt32 : public StartupOptionTyped<int32_t>
{
public:
    
    StartupOptionInt32(const std::string& iName, int32_t iVal)
    {
        name_ = iName;
        typeName_ = SO_TYPENAME_INT32;
        value_ = iVal;
        defaultValue_ = iVal;
        
        IStartupOptions::instance().add(this);
    }
    
    virtual ~StartupOptionInt32() {}
    
private:
    
};

class StartupOptionInt64 : public StartupOptionTyped<int64_t>
{
public:
    
    StartupOptionInt64(const std::string& iName, int64_t iVal)
    {
        name_ = iName;
        typeName_ = SO_TYPENAME_INT64;
        value_ = iVal;
        defaultValue_ = iVal;
        
        IStartupOptions::instance().add(this);
    }
    
    virtual ~StartupOptionInt64() {}
    
private:
    
};

class StartupOptionFloat : public StartupOptionTyped<float>
{
public:
    
    StartupOptionFloat(const std::string& iName, float iVal)
    {
        name_ = iName;
        typeName_ = SO_TYPENAME_FLOAT;
        value_ = iVal;
        defaultValue_ = iVal;
        
        IStartupOptions::instance().add(this);
    }
    
    virtual ~StartupOptionFloat() {}
    
private:
    
};

class StartupOptionString : public StartupOptionTyped<std::string>
{
public:
    
    StartupOptionString(const std::string& iName, const std::string& iVal)
    {
        name_ = iName;
        typeName_ = SO_TYPENAME_STRING;
        value_ = iVal;
        defaultValue_ = iVal;
        
        IStartupOptions::instance().add(this);
    }
    
    virtual ~StartupOptionString() {}
private:
    
};

class StartupOptions : protected IStartupOptions
{
public:
    
    /*
     <StartupOptions>
     <Option type="StartupOptionBool" name="StartupOptionBoolName" value="true"/>
     <Option type="StartupOptionInt32" name="StartupOptionInt32Name" value="0xffffffff"/f>
     <Option type="StartupOptionInt64" name="StartupOptionInt64Name" value="0xffffffffffffffff"/>
     <Option type="StartupOptionFloat" name="StartupOptionFloatName" value="3.14"/>
     <Option type="StartupOptionString" name="StartupOptionStringName" value="Hello world!"/>
     */
    
    bool initialize(const std::string& iStartupOptionsFile)
    {
        if (initalized_)
            return true;
        
#ifdef BBC_USE_BOOST
        try
        {
            // Create empty property tree object
            ptree tree;
            
            // Parse the XML into the property tree.
            read_xml(iStartupOptionsFile, tree);
            
            for (auto option : options_)
            {
                BOOST_FOREACH(ptree::value_type &node, tree.get_child(SO_STARTUPOPTIONS))
                {
                    if (node.first != SO_OPTION)
                    {
                        continue;
                    }
                    
                    std::string type = node.second.get(XMLATTR SO_DOT SO_TYPE, "");
                    std::string name = node.second.get(XMLATTR SO_DOT SO_NAME, "");
                    if (option->typeName() == type && option->name() == name)
                    {
                        // We found a match
                        // Update the know values
                        //
                        if (option->typeName() == SO_TYPENAME_BOOL)
                        {
                            bool value = false;
                            value = node.second.get(XMLATTR SO_DOT SO_VALUE, value);

                            StartupOptionBool *tmp = dynamic_cast<StartupOptionBool*>(option);
                            BBC_ASSERT(tmp);
                            tmp->set(value);
                            break;
                        }
                        else
                        if (option->typeName() == SO_TYPENAME_INT32)
                        {
                            int32_t value = 0;
                            value = node.second.get(XMLATTR SO_DOT SO_VALUE, value);

                            StartupOptionInt32 *tmp = dynamic_cast<StartupOptionInt32*>(option);
                            BBC_ASSERT(tmp);
                            tmp->set(value);
                            break;
                        }
                        else
                        if (option->typeName() == SO_TYPENAME_INT64)
                        {
                            int64_t value = 0;
                            value = node.second.get(XMLATTR SO_DOT SO_VALUE, value);

                            StartupOptionInt64 *tmp = dynamic_cast<StartupOptionInt64*>(option);
                            BBC_ASSERT(tmp);
                            tmp->set(value);
                            break;
                        }
                        else
                        if (option->typeName() == SO_TYPENAME_FLOAT)
                        {
                            float value = 0;
                            value = node.second.get(XMLATTR SO_DOT SO_VALUE, value);

                            StartupOptionFloat *tmp = dynamic_cast<StartupOptionFloat*>(option);
                            BBC_ASSERT(tmp);
                            tmp->set(value);
                            break;
                        }
                        else
                        if (option->typeName() == SO_TYPENAME_STRING)
                        {
                            std::string value;
                            value = node.second.get(XMLATTR SO_DOT SO_VALUE, value);

                            StartupOptionString *tmp = dynamic_cast<StartupOptionString*>(option);
                            BBC_ASSERT(tmp);
                            tmp->set(value);
                            break;
                        }
                        else
                        {
                            BBC_ASSERT_R(!"StartupOptions::initialize - Unknown typeName");
                        }
                    }
                }
            }

            initalized_ = true;
            
            return true;
        }
        catch(...)
        {
            std::cout << "StartupOptions::read - error reading startup options file." << std::endl;
            return false;
        }
#else
        BBC_ASSERT_R(!"StartupOptions::initialize - Requires BBC_USE_BOOST!");
#endif
        
        return false;
    }
    
    bool writeDefaultStartupOptionsFile(const std::string& iStartupOptionsFile)
    {
#ifdef BBC_USE_BOOST
        // Create an empty property tree object.
        ptree optionsTree;
        
        optionsTree.add(SO_STARTUPOPTIONS, "");

        for (auto option : options_)
        {
            if (StartupOptionBool* opt = dynamic_cast<StartupOptionBool*>(option))
            {
                BBC_ASSERT(opt->typeName() == std::string(SO_TYPENAME_BOOL));

                ptree &subtree = optionsTree.add(SO_STARTUPOPTIONS "." SO_OPTION, "");
                
                subtree.add(XMLATTR SO_DOT SO_TYPE, SO_TYPENAME_BOOL);
                subtree.add(XMLATTR SO_DOT SO_NAME, opt->name());
                subtree.add(XMLATTR SO_DOT SO_VALUE, static_cast<bool>(*opt));
            }
            
            if (StartupOptionInt32* opt = dynamic_cast<StartupOptionInt32*>(option))
            {
                BBC_ASSERT(opt->typeName() == std::string(SO_TYPENAME_INT32));
                
                ptree &subtree = optionsTree.add(SO_STARTUPOPTIONS "." SO_OPTION, "");
                
                subtree.add(XMLATTR SO_DOT SO_TYPE, SO_TYPENAME_INT32);
                subtree.add(XMLATTR SO_DOT SO_NAME, opt->name());
                subtree.add(XMLATTR SO_DOT SO_VALUE, static_cast<int32_t>(*opt));
            }

            if (StartupOptionInt64* opt = dynamic_cast<StartupOptionInt64*>(option))
            {
                BBC_ASSERT(opt->typeName() == std::string(SO_TYPENAME_INT64));
                
                ptree &subtree = optionsTree.add(SO_STARTUPOPTIONS "." SO_OPTION, "");
                
                subtree.add(XMLATTR SO_DOT SO_TYPE, SO_TYPENAME_INT64);
                subtree.add(XMLATTR SO_DOT SO_NAME, opt->name());
                subtree.add(XMLATTR SO_DOT SO_VALUE, static_cast<int64_t>(*opt));
            }

            if (StartupOptionFloat* opt = dynamic_cast<StartupOptionFloat*>(option))
            {
                BBC_ASSERT(opt->typeName() == std::string(SO_TYPENAME_FLOAT));
                
                ptree &subtree = optionsTree.add(SO_STARTUPOPTIONS "." SO_OPTION, "");
                
                subtree.add(XMLATTR SO_DOT SO_TYPE, SO_TYPENAME_FLOAT);
                subtree.add(XMLATTR SO_DOT SO_NAME, opt->name());
                subtree.add(XMLATTR SO_DOT SO_VALUE, static_cast<float>(*opt));
            }

            if (StartupOptionString* opt = dynamic_cast<StartupOptionString*>(option))
            {
                BBC_ASSERT(opt->typeName() == std::string(SO_TYPENAME_STRING));
                
                ptree &subtree = optionsTree.add(SO_STARTUPOPTIONS "." SO_OPTION, "");
                
                subtree.add(XMLATTR SO_DOT SO_TYPE, SO_TYPENAME_STRING);
                subtree.add(XMLATTR SO_DOT SO_NAME, opt->name());
                subtree.add(XMLATTR SO_DOT SO_VALUE, static_cast<std::string>(*opt));
            }
        }

        // Format and write the XML
        //
        xml_writer_settings<std::string> formatting(' ', 4);
        write_xml(iStartupOptionsFile, optionsTree, std::locale(), formatting);
        
        return true;
#else
        BBC_ASSERT_R(!"StartupOptions::writeDefaultStartupOptionsFile - Requires BBC_USE_BOOST!");
#endif
        return true;
    }
    
private:
    
    bool add(StartupOption* iOption)
    {
        BBC_ASSERT_R(iOption);
        for (auto iter : options_)
        {
            BBC_ASSERT(iOption->name() != iter->name());
        }
        
        options_.push_back(iOption);
        
        return true;
    }

    /// Guard against multiple initializations
    ///
    bool initalized_{false};
    
    std::vector<StartupOption*> options_;
};

