/*
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

#pragma once

#include <regex>
#include <fstream>

#include "BBCAssert.h"
#include "BBCMacros.h"
#include "Singleton.h"

#ifdef BBC_USE_BOOST
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sinks/async_frontend.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
#endif

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

///
/// \brief Trace is a utility class for logging information in runtime code.
///
class Trace : public Singleton<Trace>
{
public:
    
    /**
     * \brief TraceMask is a typedef for the tracing mask.
     */
    typedef uint64_t TraceMask;

    /**
     * \brief Prototype for the callback the client can install to receive trace statements.
     */
    typedef void (*TraceCallback)(const char* iMessage);

    /**
     * Priority for the trace statements.
     * Stored in the 4 most significant bits (MSB) of the TraceMask.
     */
    enum Priority : uint64_t
    {
          kPriority_Off     = 0x0000000000000000

        , kPriority_Low     = 0x1000000000000000
        , kPriority_Medium  = 0x2000000000000000
        , kPriority_High    = 0x3000000000000000

        , kPriority_Always  = 0x4000000000000000
    };

    /**
     * Category for the trace statements.
     * Stored in the 60 least significant bits (LSB) of the TraceMask.
     */
    enum Category : uint64_t
    {
          kCategory_Off                 = 0x0000000000000000
        
        , kCategory_Basic               = 0x0000000000000001
        , kCategory_CommMgr             = 0x0000000000000002

        , kCategory_Always              = 0x0FFFFFFFFFFFFFFF
    };

#define PRIORITY_TO_STRING(iVal) \
if (iPriority == iVal) \
    return STRINGIFY(iVal);
    
    /**
     * Converts a Priority to a std::string.
     * Note - asserts in debug builds if Priority is unknown.
     *
     * @param[in] iPriority to convert to std::string.
     *
     * @return std::string containing the std::string representation of the Priority
     */
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

    /**
     * Converts a std::string representation of a Priority to a Priority.
     * Note - asserts in debug builds if Priority is unknown.
     *
     * @param[in] iStr representation of a Priority to convert to Priority.
     *
     * @return Priority the priority.
     */
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
    
    /**
     * Converts a Category to a std::string.
     * Note - asserts in debug builds if Category is unknown.
     *
     * @param[in] iCategory to convert to std::string.
     *
     * @return std::string containing the std::string representation of the Category
     */
    static std::string categoryAsString(Category iCategory)
    {
        CATEGORY_TO_STRING(kCategory_Off);
        CATEGORY_TO_STRING(kCategory_Basic);
        CATEGORY_TO_STRING(kCategory_CommMgr);
        CATEGORY_TO_STRING(kCategory_Always);

        BBC_ASSERT_R(!"categoryAsString - unknown iCategory!");
        
        // Satisfy the return value
        //
        return "";
    }
    
#define STRING_TO_CATEGORY(iVal) \
if (0 == strcmp(iStr.c_str(), STRINGIFY(iVal))) \
    return iVal;
    
    /**
     * Converts a std::string representation of a Category to a Category.
     * Note - asserts in debug builds if Category is unknown.
     *
     * @param[in] iStr representation of a Category to convert to Category.
     *
     * @return Category the category.
     */
    static Category stringToCategory(const std::string& iStr)
    {
        STRING_TO_CATEGORY(kCategory_Off);
        STRING_TO_CATEGORY(kCategory_Basic);
        STRING_TO_CATEGORY(kCategory_CommMgr);
        STRING_TO_CATEGORY(kCategory_Always);

        BBC_ASSERT_R(!"stringToCategory - unknown iStr!");
        
        // Satisfy the return value
        //
        return kCategory_Off;
    }

private:
    
    ///
    /// Specialized callback for hooking into the Boost Logger layer.
    ///
    /// This callback is set to callback_ when Trace is configured to use
    /// the Boost Logger layer.
    ///
    /// When the client calls writeTrace, this callback is triggered
    /// and the trace statement is written to the Boost Logger layer.
    /// When the Boost Logger Sink::consume layer is triggered,
    /// the callback installed into boostCallback_ is triggered.
    /// This is typically the callback installed by the client.
    ///
    /// @param[in] iMessage is the message to be written
    ///
    static void BoostCallback(const char* iMessage)
    {
        BOOST_LOG_TRIVIAL(error) << iMessage << std::endl;
    }

public:
    
    /**
     * Initializes Trace using a string containing the initilization parameters
     *
     * @param[in] iTraceConfig is the configuration information
     * @param[in] iCallback the client callback for hooking into the Trace layer to recieve trace statements
     * @param[in] iUseBoost true to enable Boost Logger layer
     *
     * @return bool true when successfully initialized, false with initalization failed.
     */
    bool initializeWithBuffer(const std::string& iTraceConfig
                              , TraceCallback iCallback = nullptr
                              , bool iUseBoost = true
                              , const std::string& iBoostLogFilePath = ""
                              )
    {
        if (initalized_)
            return true;
        
        processConfig(iTraceConfig);
        
        if (iUseBoost)
        {
            callback_ = BoostCallback;
            boostCallback_ = iCallback;
            initalized_ = initBoost(iBoostLogFilePath);
        }
        else
        {
            callback_ = iCallback;
            initalized_ = true;
        }
        
        return true;
    }

    /**
     * Initializes Trace using a file containing the initilization parameters
     *
     * @param[in] iTraceConfigFile is path to the file containing configuration information
     * @param[in] iCallback the client callback for hooking into the Trace layer to recieve trace statements
     * @param[in] iUseBoost true to enable Boost Logger layer
     *
     * @return bool true when successfully initialized, false with initalization failed.
     */
    bool initializeWithFile(const std::string& iTraceConfigFile
                            , TraceCallback iCallback = nullptr
                            , bool iUseBoost = true
                            , const std::string& iBoostLogFilePath = ""
                            )
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
        
        if (iUseBoost)
        {
            callback_ = BoostCallback;
            boostCallback_ = iCallback;
            initalized_ = initBoost(iBoostLogFilePath);
        }
        else
        {
            callback_ = iCallback;
            initalized_ = true;
        }
        
        return true;
    }

    /**
     * Resets the Trace class.
     *
     * Clears all callbacks and configuration information.
     */
    void reset()
    {
#ifdef BBC_USE_BOOST
        boost::log::core::get()->remove_all_sinks();
        boostCallback_ = nullptr;
#endif
        
        initalized_ = false;
        
        traceAll_ = false;
        masks_.clear();
        
        callback_ = nullptr;
    }

    /**
     * Writes a statement to Trace
     *
     * @param[in] iMask the masking information for the statement to be traced
     * @param[in] iArgs arguments to be traced, printf style.
     */
    void writeTrace(TraceMask iMask, const char* iArgs...) const
    {
        if (!initalized_)
            return;
        
        const uint64_t priorityMask = 0xF000000000000000;
        
        // Do not trace anything explicity set to off
        //
        if ((iMask & priorityMask) == kPriority_Off)
        {
            return;
        }
        
        bool doTrace = false;
        
        // Check for the all flag being enabled
        //
        if (traceAll_)
        {
            if (traceAllPriority_ == kPriority_Off)
                return;

            if (kPriority_Always == traceAllPriority_)
                doTrace = true;

            if (iMask >= traceAllPriority_)
                doTrace = true;
        }
        
        // If we do not know about tracing yet
        // Check all of the registered masks
        //
        if (!doTrace)
        {
            for (const auto& mask : masks_)
            {
                // Check the category
                //
                if ((mask & kCategory_Always) == (iMask & kCategory_Always))
                {
                    if (kPriority_Always == (mask & priorityMask))
                    {
                        doTrace = true;
                        break;
                    }

                    // Check the priority
                    //
                    if ((iMask & priorityMask) >= (mask & priorityMask))
                    {
                        doTrace = true;
                        break;
                    }
                }
            }
        }
        
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
    
    /**
     * Processes the configuration information.
     *
     * @param[in] iTraceConfig config string to be processed
     */
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
            if ((line.length() == 0) || (line.length() && line[0] == '#'))
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
            
            // Skip any entry that is kPriority_Off
            //
            if (priority == kPriority_Off || category == kCategory_Off)
                continue;
            
            TraceMask mask = category | priority;
            
            // Filter duplicates
            //
            if (std::find (masks_.begin(), masks_.end(), mask) != masks_.end())
                continue;
            
            // Record the all category and associated priority
            // to make it easier to test for tracing
            //
            if (category == kCategory_Always)
            {
                traceAll_ = true;
                traceAllPriority_ = priority;
            }
            
            // Finally add the entry to the list
            //
            masks_.push_back(mask);
            
            std::cout << line << std::endl;
        }
    }
    
    /**
     * Initializes the Boost Logger layer.
     *
     * @param[in] iLogFilePath is path for the output file if used.
     *
     * @return true if initialized properly, false if there was a problem initializing
     */
    bool initBoost(const std::string& iLogFilePath = "")
    {
#ifdef BBC_USE_BOOST
        if (iLogFilePath.length())
        {
            logging::add_file_log
            (
             //keywords::file_name = "sample_%N.log",                                        /*< file name pattern >*/
             keywords::file_name = iLogFilePath,                                        /*< file name pattern >*/
             keywords::rotation_size = 10 * 1024 * 1024,                                   /*< rotate files every 10 MiB... >*/
             keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0), /*< ...or at midnight >*/
             keywords::auto_flush = true,
             keywords::format = "%Message%"                                 /*< log record format >*/
             );
        }
        else
        {
            struct Sink: public sinks::basic_formatted_sink_backend<char, sinks::concurrent_feeding>
            {
                void consume (const boost::log::record_view& rec, const std::string& str)
                {
                    if (Trace::instance().boostCallback_)
                    {
                        Trace::instance().boostCallback_(str.c_str());
                    }
                }
            };
            
            typedef sinks::asynchronous_sink<Sink> sink_t;
            boost::shared_ptr<sink_t> sink (new sink_t());
            boost::log::core::get()->add_sink (sink);
        }
        
        logging::core::get()->set_filter
        (
         logging::trivial::severity >= logging::trivial::info
         );

        logging::add_common_attributes();
        
        // For a trace to make sure the file is created
        //
        try
        {
            BOOST_LOG_TRIVIAL(error) << std::endl;
        }
        catch (...)
        {
            std::cerr << "Failed to create boost log file!\n";
            return false;
        }

        return true;
#endif
    }
    
    /// Size of the trace buffer to write to
    /// Any trace statement, including arguments, longer than this will be truncated.
    static const int32_t sTraceMessageSize{2048};
    
    /// The initialization state of Trace
    bool initalized_{false};
    
    /// Specialized flag to indicate all tracing is enabled
    /// This is an optimization to prevent from processing
    /// the masks_ std::vector to determine if a TraceMask is set
    bool traceAll_{false};
    
    /// Specialized flag to indicate all tracing priority is enabled
    /// This is an optimization to prevent from processing
    /// the masks_ std::vector to determine if a TraceMask is set
    Priority traceAllPriority_{kPriority_Off};

    /// List is registered TraceMasks
    /// Used to determine if a trace statement should be written
    std::vector<TraceMask> masks_;
    
    /// Pointer to the client callback.
    /// See note in BoostCallback
    TraceCallback callback_{nullptr};
    
    /// Pointer to the Boost Logger callback.
    /// See note in BoostCallback
    TraceCallback boostCallback_{nullptr};
};
