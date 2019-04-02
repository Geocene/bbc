//  Copyright (c) 2019 Bob Brown. All rights reserved.
//

#include "gtest/gtest.h"
#include "BBCAssert.h"

TEST(BBCAssertTest, BBCAssertTest_Test1)
{
    ASSERT_DEATH(BBC_ASSERT(false), "");
    ASSERT_DEATH(BBC_ASSERT_R(false), "");
    
    BBC_ASSERT(true);
    BBC_ASSERT_R(true);
}



