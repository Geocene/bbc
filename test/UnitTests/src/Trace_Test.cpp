//  Copyright (c) 2019 Bob Brown. All rights reserved.
//

#include "gtest/gtest.h"
#include "Trace.h"

static void TestTraceCallback(const char* iMessage)
{
    std::cout << iMessage << std::endl;
}

TEST(TraceTest, TraceTest_Test4)
{
    Trace::instance().initializeWithFile("BBCTrace.config");
    Trace::instance().reset();
}

TEST(TraceTest, TraceTest_Test1)
{
    Trace::instance().initializeWithBuffer("    kCategory_Basic@kPriority_Low \n   #kCategory_Basic@kPriority_High\nkCategory_Basic@kPriority_Medium"
                                           , TestTraceCallback);
    
    std::string worldStr = "world";

    BBC_TRACE(Trace::kPriority_Always | Trace::kCategory_All, "Hello world!");
    BBC_TRACE_R(Trace::kPriority_Always | Trace::kCategory_All, "Hello world!");

    BBC_TRACE(Trace::kPriority_Always | Trace::kCategory_All, "Hello %s!", worldStr.c_str());
    BBC_TRACE_R(Trace::kPriority_Always | Trace::kCategory_All, "Hello world!");

    BBC_TRACE(Trace::kPriority_Always | Trace::kCategory_All, "Hello %s - %d %f!"
              , worldStr.c_str()
              , 123
              , 3.14
              );
    BBC_TRACE_R(Trace::kPriority_Always | Trace::kCategory_All, "Hello %s - %d %f!"
              , worldStr.c_str()
              , 123
              , 3.14
              );

    int64_t test64Hex = 0xFFFFFFFFFFFFFFFF;
    BBC_TRACE(Trace::kPriority_Always | Trace::kCategory_All, "0x%jX"
              , test64Hex
              );

    BBC_TRACE(Trace::kPriority_Always | Trace::kCategory_All, "%s != %s"
              , BBC_BOOL_TO_STRING(true)
              , BBC_BOOL_TO_STRING(false)
              );

    Trace::instance().reset();
}


TEST(TraceTest, TraceTest_Test2)
{
    EXPECT_EQ(Trace::priorityAsString(Trace::kPriority_Off), "kPriority_Off");
    EXPECT_EQ(Trace::priorityAsString(Trace::kPriority_Low), "kPriority_Low");
    EXPECT_EQ(Trace::priorityAsString(Trace::kPriority_Medium), "kPriority_Medium");
    EXPECT_EQ(Trace::priorityAsString(Trace::kPriority_High), "kPriority_High");
    EXPECT_EQ(Trace::priorityAsString(Trace::kPriority_Always), "kPriority_Always");

    EXPECT_EQ(Trace::stringToPriority("kPriority_Off"), Trace::kPriority_Off);
    EXPECT_EQ(Trace::stringToPriority("kPriority_Low"), Trace::kPriority_Low);
    EXPECT_EQ(Trace::stringToPriority("kPriority_Medium"), Trace::kPriority_Medium);
    EXPECT_EQ(Trace::stringToPriority("kPriority_High"), Trace::kPriority_High);
    EXPECT_EQ(Trace::stringToPriority("kPriority_Always"), Trace::kPriority_Always);
}

TEST(TraceTest, TraceTest_Test3)
{
    EXPECT_EQ(Trace::categoryAsString(Trace::kCategory_None), "kCategory_None");
    EXPECT_EQ(Trace::categoryAsString(Trace::kCategory_Basic), "kCategory_Basic");
    EXPECT_EQ(Trace::categoryAsString(Trace::kCategory_All), "kCategory_All");
    
    EXPECT_EQ(Trace::stringToCategory("kCategory_None"), Trace::kCategory_None);
    EXPECT_EQ(Trace::stringToCategory("kCategory_Basic"), Trace::kCategory_Basic);
    EXPECT_EQ(Trace::stringToCategory("kCategory_All"), Trace::kCategory_All);
}

