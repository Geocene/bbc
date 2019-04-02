//  Copyright (c) 2019 Bob Brown. All rights reserved.
//

#include "gtest/gtest.h"
#include "BBCMacros.h"

TEST(BBCMacrosTest, BBCMacrosTest_Test1)
{
    enum MyEnum
    {
        kMyEnum_1,
        kMyEnum_2
    };
    
    EXPECT_EQ(STRINGIFY(kMyEnum_1), "kMyEnum_1");
    
#define myMacroSafe BBC_MACRO_BLOCK(int x = 1; int y = 1;)
    
    if (true)
        myMacroSafe;
    else
        int i = 1;

#define myMacrounSafe int x = 1; int y = 1;
    /*
    if (true)
        myMacrounSafe;
    else
        int i = 1;
     */

#ifdef BBC_DEBUG
    EXPECT_EQ(DEBUG, 1);
#else
    EXPECT_EQ(1, 1);
#endif
    
}
