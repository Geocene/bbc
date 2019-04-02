//  Copyright (c) 2019 Bob Brown. All rights reserved.
//

#include "gtest/gtest.h"
#include "Singleton.h"

TEST(SingletonTest, SingletonTest_Test1)
{
    class SingletonTest1 : public Singleton<SingletonTest1>
    {
    public:
        
    };

    SingletonTest1::instance();
}

