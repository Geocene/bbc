//  Copyright (c) 2019 Bob Brown. All rights reserved.
//

#pragma once

#include <regex>
#include <fstream>

#include "BBCAssert.h"
#include "BBCMacros.h"
#include "Singleton.h"

#ifdef BBC_DEBUG
#define BBC_TRACE(mask, ...) BBC_MACRO_BLOCK(Trace::instance().writeTrace(mask, __VA_ARGS__);)
#else
#define BBC_TRACE(...)
#endif

/*
 WIP...
 
#define BBC_TRACE_LINE_INFO __FILE__, STRINGIFY(: __LINE__)

#ifdef BBC_DEBUG
#define BBC_TRACE_LINE(mask, ...) BBC_MACRO_BLOCK(Trace::instance().writeTrace(mask, __FILE__, __VA_ARGS__);)
#else
#define BBC_TRACE_LINE(...)
#endif
*/

#define BBC_TRACE_R(mask, ...) BBC_MACRO_BLOCK(Trace::instance().writeTrace(mask, __VA_ARGS__);)

#define BBC_BOOL_TO_STRING(x) x ? "true" : "false"

// One of the many clever ways to print 64-bit values in hex
// 0x%jx or 0x%jX or %#018lllx or %#018lllX
//

class Trace : public Singleton<Trace>
{
public:
    
    typedef uint64_t TraceMask;
    typedef void (*TraceCallback)(const char* iMessage);

    enum Priority : uint64_t
    {
          kPriority_Off     = 0x0000000000000000

        , kPriority_Low     = 0x1000000000000000
        , kPriority_Medium  = 0x2000000000000000
        , kPriority_High    = 0x3000000000000000

        , kPriority_Always  = 0x4000000000000000
    };

    enum Category : uint64_t
    {
          kCategory_None                = 0x0000000000000000
        
        , kCategory_Basic               = 0x0000000000000001
        
        , kCategory_All                 = 0x0FFFFFFFFFFFFFFF
    };

#define PRIORITY_TO_STRING(iVal) \
if (iPriority == iVal) \
    return STRINGIFY(iVal);
    
    static std::string priorityAsString(Priority iPriority)
    {
        PRIORITY_TO_STRING(kPriority_Off);
        PRIORITY_TO_STRING(kPriority_Low);
        PRIORITY_TO_STRING(kPriority_Medium);
        PRIORITY_TO_STRING(kPriority_High);
        PRIORITY_TO_STRING(kPriority_Always);

        BBC_ASSERT(!"priorityAsString - unknown iPriority!");

        // Satisfy the return value
        //
        return "";
    }

#define STRING_TO_PRIORITY(iVal) \
if (0 == strcmp(iStr.c_str(), STRINGIFY(iVal))) \
    return iVal;

    static Priority stringToPriority(const std::string& iStr)
    {
        STRING_TO_PRIORITY(kPriority_Off);
        STRING_TO_PRIORITY(kPriority_Low);
        STRING_TO_PRIORITY(kPriority_Medium);
        STRING_TO_PRIORITY(kPriority_High);
        STRING_TO_PRIORITY(kPriority_Always);

        BBC_ASSERT(!"stringToPriority - unknown iStr!");
        
        // Satisfy the return value
        //
        return kPriority_Off;
    }

#define CATEGORY_TO_STRING(iVal) \
if (iCategory == iVal) \
    return STRINGIFY(iVal);
    
    static std::string categoryAsString(Category iCategory)
    {
        CATEGORY_TO_STRING(kCategory_None);
        CATEGORY_TO_STRING(kCategory_Basic);
        CATEGORY_TO_STRING(kCategory_All);
        
        BBC_ASSERT_R(!"categoryAsString - unknown iCategory!");
        
        // Satisfy the return value
        //
        return "";
    }
    
#define STRING_TO_CATEGORY(iVal) \
if (0 == strcmp(iStr.c_str(), STRINGIFY(iVal))) \
    return iVal;
    
    static Category stringToCategory(const std::string& iStr)
    {
        STRING_TO_CATEGORY(kCategory_None);
        STRING_TO_CATEGORY(kCategory_Basic);
        STRING_TO_CATEGORY(kCategory_All);
        
        BBC_ASSERT_R(!"stringToCategory - unknown iStr!");
        
        // Satisfy the return value
        //
        return kCategory_None;
    }

    bool initializeWithBuffer(const std::string& iTraceConfig, TraceCallback iCallback = nullptr, bool iUseBoost = true)
    {
        if (initalized_)
            return true;
        
        processConfig(iTraceConfig);
        
        callback_ = iCallback;
        initalized_ = true;
        
        return true;
    }

    bool initializeWithFile(const std::string& iTraceConfigFile, TraceCallback iCallback = nullptr, bool iUseBoost = true)
    {
        if (initalized_)
            return true;
        
        std::ifstream fileStream;
        fileStream.open(iTraceConfigFile);
        
        // See if the file exists
        //
        if (!fileStream.is_open())
            return false;

        std::stringstream buffer;
        buffer << fileStream.rdbuf();
        processConfig(buffer.str());
        
        fileStream.close();
        
        callback_ = iCallback;
        initalized_ = true;
        
        return true;
    }

    void reset()
    {
        initalized_ = false;
        
        traceAll_ = false;
        masks_.clear();
        
        callback_ = nullptr;
    }

    void writeTrace(TraceMask iMask, const char* iArgs...) const
    {
        bool doTrace = false;
        
        if (iMask)
            doTrace = true;
        
        if (doTrace)
        {
            va_list argList;
            va_start(argList, iArgs);
            
            // Buffer to print the message to,
            // must include terminiating character
            //
            char traceMessage[sTraceMessageSize];
            memset(traceMessage, 0, sTraceMessageSize);
            
            vsnprintf(traceMessage, sTraceMessageSize, iArgs, argList);
            
            va_end(argList);


            if (callback_)
            {
                callback_(traceMessage);
            }
            else
            {                
                std::cout << traceMessage << std::endl;
            }
        }
    }
    
private:
    
    void processConfig(const std::string& iTraceConfig)
    {
        std::stringstream ss(iTraceConfig);
        std::string line;
        while (std::getline(ss, line))
        {
            // Trim leading and trailing spaces
            // Fancy regex trimming
            //
            line = std::regex_replace(line, std::regex("^ +| +$|( ) +"), "$1");

            // Check for # indicating a commented out value
            //
            if (line.length() && line[0] == '#')
                continue;
            
            // Split at the @
            //
            std::string categoryStr = line.substr(0, line.find("@"));
            std::string priorityStr = line.substr(line.find("@") + 1, line.length());

            // Trim leading and trailing spaces
            //
            categoryStr = std::regex_replace(categoryStr, std::regex("^ +| +$|( ) +"), "$1");
            priorityStr = std::regex_replace(priorityStr, std::regex("^ +| +$|( ) +"), "$1");

            Category category = stringToCategory(categoryStr);
            Priority priority = stringToPriority(priorityStr);
            
            TraceMask mask = category | priority;
            
            // Filter duplicates
            //
            if (std::find (masks_.begin(), masks_.end(), mask) != masks_.end())
                continue;
            
            // Finally add the entry to the list
            //
            masks_.push_back(mask);
            
            std::cout << line << std::endl;
        }
    }
    
    static const int32_t sTraceMessageSize{2048};
    
    bool initalized_{false};
    
    bool traceAll_{false};
    std::vector<TraceMask> masks_;
    
    TraceCallback callback_{nullptr};
};
