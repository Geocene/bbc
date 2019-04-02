//  Copyright (c) 2019 Bob Brown. All rights reserved.
//

#include "gtest/gtest.h"

class BBCEnvironment : public ::testing::Environment
{
public:
    virtual void SetUp()
    {
        
    }
};

::testing::Environment* const myEnvironment = ::testing::AddGlobalTestEnvironment(new BBCEnvironment);
