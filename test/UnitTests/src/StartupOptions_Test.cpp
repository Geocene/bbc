//  Copyright (c) 2019 Bob Brown. All rights reserved.
//

#include "gtest/gtest.h"
#include "StartupOptions.h"

#define StartupOptionBoolTest1_Value true
StartupOptionBool StartupOptionBoolTest1("StartupOptionBoolTest1", StartupOptionBoolTest1_Value);

#define StartupOptionBoolTest2_Value false
StartupOptionBool StartupOptionBoolTest2("StartupOptionBoolTest2", StartupOptionBoolTest2_Value);

#define StartupOptionInt32Test1_Value 123
StartupOptionInt32 StartupOptionInt32Test1("StartupOptionInt32Test1", StartupOptionInt32Test1_Value);

#define StartupOptionInt32Test2_Value -456
StartupOptionInt32 StartupOptionInt32Test2("StartupOptionInt32Test2", StartupOptionInt32Test2_Value);

#define StartupOptionInt64Test1_Value 123456
StartupOptionInt64 StartupOptionInt64Test1("StartupOptionInt64Test1", StartupOptionInt64Test1_Value);

#define StartupOptionInt64Test2_Value -123456
StartupOptionInt64 StartupOptionInt64Test2("StartupOptionInt64Test2", StartupOptionInt64Test2_Value);

#define StartupOptionFloatTest1_Value 3.14
StartupOptionFloat StartupOptionFloatTest1("StartupOptionFloatTest1", StartupOptionFloatTest1_Value);

#define StartupOptionFloatTest2_Value -3.14
StartupOptionFloat StartupOptionFloatTest2("StartupOptionFloatTest2", StartupOptionFloatTest2_Value);

#define StartupOptionStringTest1_Value "my string 1"
StartupOptionString StartupOptionStringTest1("StartupOptionStringTest1", StartupOptionStringTest1_Value);

#define StartupOptionStringTest2_Value "my string 2"
StartupOptionString StartupOptionStringTest2("StartupOptionStringTest2", StartupOptionStringTest2_Value);


TEST(StartupOptionTest, StartupOptionTest_WriteReadDefaults)
{
    EXPECT_EQ(IStartupOptions::instance().writeDefaultStartupOptionsFile("StartupOptions.default.xml"), true);

    EXPECT_EQ(IStartupOptions::instance().initialize("StartupOptions.default.xml"), true);
    
    EXPECT_EQ(static_cast<bool>(StartupOptionBoolTest1), StartupOptionBoolTest1_Value);
    EXPECT_EQ(static_cast<bool>(StartupOptionBoolTest2), StartupOptionBoolTest2_Value);
    
    EXPECT_EQ(static_cast<int32_t>(StartupOptionInt32Test1), StartupOptionInt32Test1_Value);
    EXPECT_EQ(static_cast<int32_t>(StartupOptionInt32Test2), StartupOptionInt32Test2_Value);
    
    EXPECT_EQ(static_cast<int64_t>(StartupOptionInt64Test1), StartupOptionInt64Test1_Value);
    EXPECT_EQ(static_cast<int64_t>(StartupOptionInt64Test2), StartupOptionInt64Test2_Value);
    
    EXPECT_NEAR(static_cast<float>(StartupOptionFloatTest1), StartupOptionFloatTest1_Value, 0.00001);
    EXPECT_NEAR(static_cast<float>(StartupOptionFloatTest2), StartupOptionFloatTest2_Value, 0.00001);
    
    EXPECT_EQ(static_cast<std::string>(StartupOptionStringTest1), StartupOptionStringTest1_Value);
    EXPECT_EQ(static_cast<std::string>(StartupOptionStringTest2), StartupOptionStringTest2_Value);
}

TEST(StartupOptionTest, StartupOptionTest_ReadDefaults2X)
{
    EXPECT_EQ(IStartupOptions::instance().writeDefaultStartupOptionsFile("StartupOptions.default.xml"), true);

    EXPECT_EQ(IStartupOptions::instance().initialize("StartupOptions.default.xml"), true);
    EXPECT_EQ(IStartupOptions::instance().initialize("StartupOptions.default.xml"), true);
}
